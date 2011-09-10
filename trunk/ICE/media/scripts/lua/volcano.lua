-- Function for mini Enemy Type
-- param: actual state
-- return: new state
function VolcanoLogic( pEnemy )
-- 	STOPPED
	if pEnemy:getState() == enemy.STOPPED then
		if pEnemy:isActive() then
			pEnemy:setState(enemy.ATTACK)
		elseif not pEnemy:isAlive() then
			pEnemy:setState(enemy.INACTIVE)
		end
--	ATTACK
	elseif pEnemy:getState() == enemy.ATTACK then
		if not pEnemy:isAlive() then
			pEnemy:setState(enemy.DYING)	
		end
-- 	DYING
	elseif pEnemy:getState() == enemy.DYING then
		if pEnemy:isAnimDyingEnded() then
			pEnemy:setState(enemy.DEAD)
		end
-- 	DEAD, After DYING
	elseif pEnemy:getState() == enemy.DEAD then
		pEnemy:setState(enemy.INACTIVE)
	end
	return pEnemy:getState()
end