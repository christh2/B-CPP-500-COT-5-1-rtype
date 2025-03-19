function Update(vel, dt)
    local amplitude = 1.5
    local frequency = 1 / 4
    vel.speed_y = amplitude * math.sin(2 * math.pi * frequency * vel.time)
    vel.speed_y = math.max(-amplitude, math.min(amplitude, vel.speed_y))
    vel.speed_x = 0.03
    vel.dir_x = -1
    vel.dir_y = 1
    if vel.time > 1000000 then
        vel.time = 0
    end
    vel.time = vel.time + dt
    return vel
end

function Behavior(time)
    local cycle = time * 1000
    if cycle % 5 == 0 then
        return "shoot enem_weapon 33 10"
    else
        return ""
    end
end
