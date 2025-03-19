function Update(pos, links, dt)
    pos.speed_y = 0
    pos.speed_x = 3
    pos.dir_x = -1
    pos.dir_y = 1
--    pos.time = pos.time + dt
    -- if (pos.time * 1000) > 100000 then
    --     pos.time = 0
    -- end
    return pos
end

function Behavior(vel, links, time)
    local cycle = math.floor(time / 1000)
    if cycle % 2 == 0 then
        local x = tostring(math.floor(vel.posx - 20))
        local y = tostring(math.floor(vel.posy + 150))
        local command = "shoot target_weapon " .. x .. " " .. y .. " ../assets/scripts/target.lua"
        return command
    else
        return ""
    end
end
