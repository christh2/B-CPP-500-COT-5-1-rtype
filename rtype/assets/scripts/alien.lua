function Update(pos, links, dt)
    pos.speed_y = 0
    pos.speed_x = 0
    pos.dir_x = -1
    pos.dir_y = 1
    if #links > 0 then
        local dy = links[1].posy - pos.posy
        if dy > 8 then
            pos.speed_y = 0.3
            pos.dir_y = 1
        elseif dy < -8 then
            pos.speed_y = 0.3
            pos.dir_y = -1
        end
    end
    if pos.posx > 800 then
        pos.speed_x = 1
        pos.dir_x = -1
    else
        pos.speed_x = 0
        pos.dir_x = 0
    end
    return pos
end

function Behavior(vel, links, time)
    local cycle = math.floor(time / 1000)
    if #links > 0 then
        local dy = links[1].posy - vel.posy
        if cycle % 3 == 0 then
            local x = tostring(math.floor(vel.posx + 33))
            local y = tostring(math.floor(vel.posy + 10))
            local command = "shoot enem_weapon " .. x .. " " .. y .. " ../assets/scripts/basic_weapon.lua"
            return command
        else
            return ""
        end
    end
    if cycle % 6 == 0 then
        local x = tostring(math.floor(vel.posx + 33))
        local y = tostring(math.floor(vel.posy + 10))
        local command = "shoot enem_weapon " .. x .. " " .. y .. " ../assets/scripts/basic_weapon.lua"
        return command
    else
        return ""
    end
end
