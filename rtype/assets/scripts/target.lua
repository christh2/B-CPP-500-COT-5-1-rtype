function Update(pos, links, dt)
    pos.dir_x = 1
    pos.dir_y = 1

    if #links > 0 then
        local dx = links[1].posx - pos.posx
        local dy = links[1].posy - pos.posy
        local distance = math.sqrt(dx * dx + dy * dy)

        if (links[1].posx > pos.posx) then
            pos.speed_x = -3
            pos.speed_y = 0
            return pos
        end
        if distance > 0 then
            dx = dx / distance
            dy = dy / distance
            pos.speed_x = dx * 3 * (dt / 1000)
            pos.speed_y = dy * 3 * (dt / 1000)
        end
    end

    return pos
end

function Behavior(pos, links, time)
    return ""
end
