-- Function for smart Enemy Type
-- param: actual state
-- return: new state
function smartLogic( pEnemy )
	raMax = 550
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
			pEnemy:setState(enemy.INACTIVE)
			elseif (ra  < raMax) and pEnemy:isVisiblePlayerCam() and not pEnemy:isVisibleWideCam() then
				pEnemy:setState(enemy.ATTACK)
				elseif (ra  > raMax) and not pEnemy:isVisiblePlayerCam()  then
					pEnemy:setState(enemy.INACTIVE)
			
		end
--	ATTACK
	elseif pEnemy:getState() == enemy.ATTACK then
		ra = pEnemy:rangeAttack()
		if not pEnemy:isAlive() then
			pEnemy:setState(enemy.DYING)
		elseif (ra  > raMax) or (ra  < 10) then 
			pEnemy:setState(enemy.FOLLOWING_TRAJECTORY)
			elseif not pEnemy:isVisiblePlayerCam() or pEnemy:isVisibleWideCam() then 
				pEnemy:setState(enemy.FOLLOWING_TRAJECTORY)		
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