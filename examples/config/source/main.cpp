//
// Created by cpasjuste on 08/12/16.
//

#include "cross2d/c2d.h"

using namespace c2d;
using namespace c2d::config;

void addConfigSection(Config *config) {

    Section section("DEMO");
    section.addOption({"INTEGER", 10});
    section.addOption({"STRING", "Hello World"});
    section.addOption({"FLOAT", 9.99f});
    config->addSection(section);
}

int main() {

    // create the main renderer
    auto *renderer = new C2DRenderer({C2D_SCREEN_WIDTH, C2D_SCREEN_HEIGHT});
    renderer->setClearColor(Color::GrayLight);

    // create a configuration named "C2D_CFG" in a writable directory (getHomePath)
    Config *config = new Config("C2D_CFG", renderer->getIo()->getHomePath() + "config.cfg");
    // add a section to the configuration
    addConfigSection(config);
    // load the configuration from file, overwriting default values (added in addConfigSection)
    if (!config->load()) {
        // file doesn't exist or is malformed, (re)create it
        config->save();
    }

    // here the "INTEGER" option in section "DEMO" will be overwritten by the "config.cfg" file parameter, if any
    printf("DEMO->INTEGER = %i\n", config->getOption("DEMO", "INTEGER")->getInteger());

    // loop through all sections and options
    for (auto &section : *config->getSections()) {
        printf("\n%s:\n", section.getName().c_str());
        for (auto &option : *section.getOptions()) {
            if (option.getType() == Option::Type::Integer) {
                printf("\t%s: %i\n", option.getName().c_str(), option.getInteger());
            } else if (option.getType() == Option::Type::Float) {
                printf("\t%s: %f\n", option.getName().c_str(), option.getFloat());
            } else {
                printf("\t%s: %s\n", option.getName().c_str(), option.getString().c_str());
            }
        }
    }

    // change an option and save the new configuration, next time we load this example
    // the "INTEGER" from section "DEMO" option will return "20"
    config->getOption("DEMO", "INTEGER")->setInteger(20);
    config->save();

    // main loop
    while (true) {

        // stop if any key is pressed
        if (renderer->getInput()->getKeys()) {
            break;
        }

        // draw everything
        renderer->flip();
    }

    // cleanup
    delete (config);
    // will delete child's (textures, shapes, text..)
    delete (renderer);

    return 0;
}
