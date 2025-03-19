function Update(vel, links, dt)
    local amplitude = 2.5
    local frequency = 0.03

    vel.speed_y = amplitude * math.sin(2 * math.pi * frequency * (vel.t))
    vel.speed_y = math.max(-amplitude, math.min(amplitude, vel.speed_y))

    if vel.life > 100 then
        if vel.posx < 800 then
            vel.speed_x = 0
        elseif vel.posx > 800 then
            vel.speed_x = -2
        end
    end

    if vel.life < 100 then
        if vel.posx < 800 then
            vel.speed_x = 0
        elseif vel.posx > 800 then
            vel.speed_x = 2
        end
    end

    vel.dir_x = 1
    vel.dir_y = 1
    vel.t = vel.t + vel.dt
    return vel
end

function Behavior(vel, links, time)
    local cycle = math.floor(time / 1000)
    local y_min = math.floor(vel.posy or 0) + 20
    local y_max = math.floor(y_min + 500)
    local y_range = math.random(y_min, y_max)
    local final_command = {}

    if vel.posx > 800 then
        return ""
    end
    if cycle % 6 == 0 then
        local x = tostring(math.floor(vel.posx + 50))
        local y = tostring(math.floor(y_range))
        local command = "shoot enem_weapon " .. x .. " " .. y .. " ../assets/scripts/basic_weapon.lua"
        local command_1 = "shoot enem_weapon " .. x .. " " .. y .. " ../assets/scripts/ob_up_weapon.lua"
        local command_4 = "shoot enem_weapon " .. x .. " " .. y .. " ../assets/scripts/ob_down_weapon.lua"
        table.insert(final_command, command)
        table.insert(final_command, command_1)
        table.insert(final_command, command_4)
    end
    local result = table.concat(final_command, ";")
    return result
end
