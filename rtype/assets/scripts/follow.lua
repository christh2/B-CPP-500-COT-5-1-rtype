function Update(pos, links, dt)
    pos.speed_y = 0
    pos.speed_x = 0
    pos.dir_x = 0
    pos.dir_y = 0
    if #links > 0 then
        pos.speed_x = links[1].speed_x
        pos.speed_y = links[1].speed_y
        pos.dir_x = links[1].dir_x
        pos.dir_y = links[1].dir_y        
    end
    if #links == 0 then
        pos.t = pos.t or 0
        pos.dt = pos.dt or dt
        local amplitude = 3
        local frequency = 1 / 4
        pos.speed_y = amplitude * math.sin(2 * math.pi * frequency * (pos.t))
        pos.speed_y = math.max(-amplitude, math.min(amplitude, pos.speed_y))
        pos.speed_x = 4
        pos.dir_x = -1
        pos.dir_y = 1
        pos.t = pos.t + pos.dt
    end
    return pos
end

function Behavior(vel, links, time)
    return ""
end