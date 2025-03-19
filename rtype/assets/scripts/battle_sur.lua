function Update(pos, dt)
    pos.speed_y = 0
    pos.speed_x = 1
    pos.dir_x = -1
    pos.dir_y = 1
    pos.time = pos.time + dt
    if pos.time > 1000000 then
        pos.time = 0
    end
    return pos
end

function Behavior(time)
    local cycle = time * 1000
    if cycle % 64 == 0 then
        return "shoot enem_weapon 33 0"
    else
        return ""
    end
end
