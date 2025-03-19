#pragma once
#include <iostream>
#include <sol/sol.hpp>
#include <chrono>
#include <cmath>

namespace NovaEngine {
    typedef struct behavior_t {
        static sol::state lua;
        sol::function update;
        sol::function behavior;
        std::chrono::high_resolution_clock::time_point clk;
        std::chrono::high_resolution_clock::time_point t1;
        std::chrono::high_resolution_clock::time_point t2;
        double time;
        double dt;
        double t;
        double dt_2;

        behavior_t(std::string script, double delta, double _delay = 500) : clk(std::chrono::high_resolution_clock::now()) {
            lua.script_file(script);
            try {
                update = lua["Update"];
                behavior = lua["Behavior"];
                if (!update.valid() || !behavior.valid()) {
                    throw sol::error("Invalid script");
                }
            } catch (const sol::error& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            time = 0;
            t1 = std::chrono::high_resolution_clock::now();
            t2 = std::chrono::high_resolution_clock::now();
            dt = delta;
            t = 0;
            dt_2 = _delay;
        }

        bool delay() {
            t1 = std::chrono::high_resolution_clock::now();
            time = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - clk).count();
            if (time > 120000)
                clk = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t2).count() > dt_2) {
                t2 = std::chrono::high_resolution_clock::now();
                return true;
            } else {
                return false;
            }
        }

    } behavior_t;
}

