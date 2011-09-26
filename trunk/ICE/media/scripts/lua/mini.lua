-- Function for mini Enemy Type
-- param: actual state
-- return: new state
function miniLogic( pEnemy )
	raMax = 400
-- 	STOPPED
	if pEnemy:getState() == enemy.STOPPED then
		if pEnemy:isActive() then
			pEnemy:setState(enemy.FOLLOWING_TRAJECTORY)
		elseif not pEnemy:isAlive() then
			pEnemy:setState(enemy.INACTIVE)
		end
-- 	MOVING
	elseif pEnemy:getState() == enemy.FOLLOWING_TRAJECTORY then
		ra = pEnemy:rangeAttack()
		if not pEnemy:isAlive() then
			pEnemy:setState(enemy.START_DYING)
		elseif (ra  < raMax) and not pEnemy:isVisibleWideCam() then
			pEnemy:setState(enemy.ATTACK)
		elseif (ra  > raMax) and pEnemy:isVisibleWideCam()  then
			pEnemy:setState(enemy.INACTIVE)
		end
--	ATTACK
	elseif pEnemy:getState() == enemy.ATTACK then
		ra = pEnemy:rangeAttack()
		if not pEnemy:isAlive() then
			pEnemy:setState(enemy.START_DYING)
		--elseif (ra  > raMax) then 
		--	pEnemy:setState(enemy.FOLLOWING_TRAJECTORY)
		elseif not pEnemy:isVisiblePlayerCam() or pEnemy:isVisibleWideCam() then 
			pEnemy:setState(enemy.INACTIVE)
		end
-- START_DYING
	elseif pEnemy:getState() == enemy.START_DYING then
		pEnemy:setState(enemy.DYING)
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