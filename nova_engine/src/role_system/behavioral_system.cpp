#include <iostream>
#include "../../include/core/system.hpp"
#include "../../include/core/system.hpp"
#include "../../include/core/entity.hpp"
#include "../../include/core/hybrid_array.hpp"

namespace NovaEngine {
    sol::state behavior_t::lua;
    void behavior_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<velocity_t>& velocities,
    hybrid_array<behavior_t>& scripts, hybrid_array<position_t>& positions, hybrid_array<linked_t>& links,
    hybrid_array<actor_t>& actors) {
        sol::table lua_vel = behavior_t::lua.create_table();
        // std::vector<position_t> friend_pos;
        for (auto &i : entities) {
//            friend_pos.clear();
            auto &behavior = scripts[i];
            auto &vel = velocities[i];
            auto &pos = positions[i];
            if (behavior.has_value() && vel.has_value() && pos.has_value()) {
                auto &script = behavior.value();
                auto &vel_vlue = vel.value();
                lua_vel["dir_x"] = vel_vlue.dir_x;
                lua_vel["dir_y"] = vel_vlue.dir_y;
                lua_vel["speed_x"] = vel_vlue.speed_x;
                lua_vel["speed_y"] = vel_vlue.speed_y;
                lua_vel["posx"] = pos.value()._x;
                lua_vel["posy"] = pos.value()._y;
                lua_vel["dt"] = script.dt;
                lua_vel["t"] = script.t;
                if (actors[i].has_value()) {
                    lua_vel["actor"] = actors[i].value().name.c_str();
                    lua_vel["life"] = actors[i].value().live;
                    //std::cout << actors[i].value().live << std::endl;
                } else {
                    lua_vel["actor"] = "unknown";
                    lua_vel["life"] = 0;
                }
                sol::table lua_friend = behavior_t::lua.create_table();
                if (links[i].has_value()) {
                    auto &link = links[i].value();
                    if (!link.lock) {
                        for (auto &id : link.links) {
                            if (positions[id].has_value()) {
                                link.friend_pos[id] = positions[id].value();
                            }
                            if (velocities[id].has_value()) {
                                link.friend_vel[id] = velocities[id].value();
                            }
                        }
                    }
                    for (int x = 0; x < link.links.size(); x++) {
                        int index = link.links[x];
                        sol::table tmp = behavior_t::lua.create_table();
                        if (positions[index].has_value()) {
                            tmp["posx"] = link.friend_pos[index]._x;
                            tmp["posy"] = link.friend_pos[index]._y;
                        } else {
                            tmp["posx"] = 0;
                            tmp["posy"] = 0;
                        }
                        if (velocities[index].has_value()) {
                            tmp["dir_x"] = link.friend_vel[index].dir_x;
                            tmp["dir_y"] = link.friend_vel[index].dir_y;
                            tmp["speed_x"] = link.friend_vel[index].speed_x;
                            tmp["speed_y"] = link.friend_vel[index].speed_y;
                        } else {
                            tmp["dir_x"] = 0;
                            tmp["dir_y"] = 0;
                            tmp["speed_x"] = 0;
                            tmp["speed_y"] = 0;
                        }
                        if (actors[x].has_value()) {
                            tmp["actor"] = actors[x].value().name.c_str();
                            tmp["life"] = actors[x].value().live;
                        } else {
                            tmp["actor"] = "unknown";
                            tmp["life"] = 0;
                        }
                        lua_friend[x + 1] = tmp;
                    }
                }
                sol::table updated_vel = script.update(lua_vel, lua_friend, script.time);
                vel_vlue.dir_x = updated_vel["dir_x"];
                vel_vlue.dir_y = updated_vel["dir_y"];
                vel_vlue.speed_x = updated_vel["speed_x"];
                vel_vlue.speed_y = updated_vel["speed_y"];
                script.t = updated_vel["t"];
                if (script.delay()) {
                    try {
                        std::string order = script.behavior(lua_vel, lua_friend, script.time);
                        if (order.empty())
                            continue;
                        std::stringstream ss(order);
                        std::string order_str;
                        while (std::getline(ss, order_str, ';')) {
                            if (!order.empty()) {
                                message_queue.push_back("Behavior order:" + order_str + " " + std::to_string(i));
                            }
                        }
                    } catch (const sol::error& e) {
                        std::cerr << "Lua runtime error: " << e.what() << std::endl;
                    }
                }
            }
        }
    }
}
