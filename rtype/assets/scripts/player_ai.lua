function Update(pos, links, dt)
    pos.speed_y = 0
    pos.speed_x = 0
    pos.dir_x = -1
    pos.dir_y = 1

    -- local flee_radius = 50

    -- for _, link in ipairs(links) do
    --     local dx = link.posx - pos.posx
    --     local dy = link.posy - pos.posy
    --     local distance = math.sqrt(dx * dx + dy * dy)

    --     if link.actor == "unknown" and distance <= flee_radius then
    --         if dx > 0 then
    --             pos.dir_x = -1
    --         elseif dx < 0 then
    --             pos.dir_x = 1
    --         end

    --         if dy > 0 then
    --             pos.dir_y = -1
    --         elseif dy < 0 then
    --             pos.dir_y = 1
    --         end

    --         pos.speed_x = 0.5
    --         pos.speed_y = 0.5
    --         return pos
    --     end
    -- end

    for _, link in ipairs(links) do
        print(_, link.actor)
        if link.actor ~= "Player" and link.actor ~= "unknown" then
            local dy = links[_].posy - pos.posy
            if dy > 8 then
                pos.speed_y = 0.3
                pos.dir_y = 1
            elseif dy < -8 then
                pos.speed_y = 0.3
                pos.dir_y = -1
            end
            return pos
        end
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
            local command = "shoot basic_weapon " .. x .. " " .. y .. " ../assets/scripts/player_weapon.lua"
            return command
        else
            return ""
        end
    end
    if cycle % 6 == 0 then
        local x = tostring(math.floor(vel.posx + 33))
        local y = tostring(math.floor(vel.posy + 10))
        local command = "shoot long_Shoot_2 " .. x .. " " .. y .. " ../assets/scripts/player_weapon.lua"
        return command
    else
        return ""
    end
end
