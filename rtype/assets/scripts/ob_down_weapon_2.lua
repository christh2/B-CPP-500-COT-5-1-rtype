function Update(pos, links, dt)
    pos.speed_y = 1
    pos.speed_x = 3
    pos.dir_x = -1
    pos.dir_y = -1
    if pos.posy < 0 then
        pos.dir_y = 1
    end
    if pos.posy > 1000 then
        pos.dir_y = -1
    end
    return pos
end

function Behavior(vel, links, time)
    return ""
end