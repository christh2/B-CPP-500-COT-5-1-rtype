function Update(vel, links, dt)
    vel.t = vel.t or 0
    vel.dt = vel.dt or dt

    local amplitude = 1.5
    local frequency = 0.03
    vel.speed_y = amplitude * math.cos(2 * math.pi * frequency * (vel.t))
    vel.speed_y = math.max(-amplitude, math.min(amplitude, vel.speed_y))
    vel.speed_x = 2
    vel.dir_x = -1
    vel.dir_y = 1
    vel.t = vel.t + vel.dt
    return vel
end

function Behavior(vel, links, time)
    vel.posx = vel.posx or 0
    vel.posy = vel.posy or 0
    local cycle = time % 5000
    if (cycle > 1000 and cycle < 2500) then
        local x = tostring(math.floor(vel.posx + 33))
        local y = tostring(math.floor(vel.posy + 10))
        local command = "shoot enem_weapon " .. x .. " " .. y .. " ../assets/scripts/basic_weapon.lua"
        return command
    else
        return ""
    end
end
