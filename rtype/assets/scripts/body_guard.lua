function Update(vel, dt)
    local amplitude = 3
    local frequency = 0.07

    vel.speed_y = amplitude * math.sin(2 * math.pi * frequency * (vel.t))
    vel.speed_y = math.max(-amplitude, math.min(amplitude, vel.speed_y))

    if vel.posx < 800 then
        vel.speed_x = 0
    else
        vel.speed_x = 2
    end

    vel.dir_x = -1
    vel.dir_y = 1
    vel.t = vel.t + vel.dt
    return vel
end

function Behavior(vel, time)
    return ""
end
