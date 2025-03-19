function Update(pos, links, dt)
    pos.speed_y = 0
    pos.speed_x = 3
    pos.dir_x = 1
    pos.dir_y = 1
    return pos
end

function Behavior(vel, links, time)
    return ""
end