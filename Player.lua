local Player = {
    size = 1, -- 3 mushrooms sizes
    x = 50,
    y = 50,
    image = love.graphics.newImage("imgs/player.png"),
    quadsData = {
        {
            offsetX = 0,
            offsetY = 0,
            width = 16,
            height = 16,
            frames = {
                idleRight = {
                    quads = {},
                    n_frames = 4,
                    frame_speed = 0.3
                },
                walkingRight = {
                    quads = {},
                    n_frames = 4,
                    frame_speed = 0.1
                },
                idleLeft = {
                    quads = {},
                    n_frames = 4,
                    frame_speed = 0.3
                },
                walkingLeft = {
                    quads = {},
                    n_frames = 4,
                    frame_speed = 0.1
                },
            },
            frameTypes = {"idleRight", "walkingRight", "idleLeft", "walkingLeft"}
        }
    },
    currentAnimation = "idleRight",
    animationIndex = 1,
    frameRate = 0.10,
    frameTimer = 0,
    currentQuad = nil,
    direction = "right",
    walking = false
}

local frameCounter = 0
 
for k, frameT in ipairs(Player.quadsData[1].frameTypes) do
    
    if k > 1 then   
        frameCounter = frameCounter + Player.quadsData[1].frames[Player.quadsData[1].frameTypes[k - 1]].n_frames
    end

    for i = 1, Player.quadsData[1].frames[frameT].n_frames do

        local actualIndex = i + frameCounter

        table.insert(Player.quadsData[1].frames[frameT].quads,
            love.graphics.newQuad(
                (actualIndex - 1) * Player.quadsData[1].width,
                0,
                Player.quadsData[1].width,
                Player.quadsData[1].height,
                Player.image:getDimensions() 
            )
        )
    end
end


function Player:draw()
    love.graphics.draw(self.image, self.currentQuad, self.x, self.y)
end

function Player:update(dt)
    self.currentQuad = self.quadsData[self.size].frames[self.currentAnimation].quads[self.animationIndex]
    self.frameTimer = self.frameTimer - dt
    if self.frameTimer < 0 then
        self.frameTimer = self.quadsData[self.size].frames[self.currentAnimation].frame_speed
        self.animationIndex = (self.animationIndex % self.quadsData[self.size].frames[self.currentAnimation].n_frames) + 1
    end

    self:controlsUpdate(dt)

end

function Player:controlsUpdate(dt)
    -- do not react if pressing opposite buttons
    if not (love.keyboard.isDown("right") and love.keyboard.isDown("left")) and not (love.keyboard.isDown("up") and love.keyboard.isDown("down")) then
        -- there must be a better way to do this
        if love.keyboard.isDown("right") then 

            if (not self.walking) or (not (self.direction == "right")) then
                self:changeAnimation("walkingRight")
                self.direction = "right"
                self.walking = true
            end

            self.x = self.x + 50 * dt
        end
        if love.keyboard.isDown("left") then 

            if (not self.walking) or (not (self.direction == "left")) then 
                self:changeAnimation("walkingLeft")
                self.direction = "left"
                self.walking = true
            end

            self.x = self.x - 50 * dt
        end
        if love.keyboard.isDown("down") then
            self.y = self.y + 50 * dt
            if not self.walking then
                self.walking = true
                if self.direction == "left" then
                    self:changeAnimation("walkingLeft")
                else
                    self:changeAnimation("walkingRight")
                end            
            end
        end
        if love.keyboard.isDown("up") then
            self.y = self.y - 50 * dt
            if not self.walking then
                self.walking = true
                if self.direction == "left" then
                    self:changeAnimation("walkingLeft")
                else
                    self:changeAnimation("walkingRight")
                end            
            end
        end
    end

    if (not love.keyboard.isDown("right")) and (not love.keyboard.isDown("left"))
        and (not love.keyboard.isDown("down")) and (not love.keyboard.isDown("up")) 
        or (love.keyboard.isDown("right") and love.keyboard.isDown("left")) or 
        (love.keyboard.isDown("up") and love.keyboard.isDown("down")) then
        if self.walking then
            if self.direction == "right" then
                self:changeAnimation("idleRight")
            else
                self:changeAnimation("idleLeft")
            end
        end
        self.walking = false
    end
end

function Player:changeAnimation(newAnimation)
    self.currentAnimation = newAnimation
    self.frameTimer = 0 
    self.animationIndex = 1
end

return Player