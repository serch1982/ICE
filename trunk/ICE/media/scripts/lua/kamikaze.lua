-- Function for Kamikaze Enemy Type
-- param: actual state
-- return: new state
function KamikazeLogic( pEnemy )
-- 	STOPPED
	if pEnemy:getState() == enemy.STOPPED then
		ra = pEnemy:rangeAttack()
		if pEnemy:isActive() and (ra  < 1500) then
			pEnemy:setState(enemy.ATTACK)
		elseif not pEnemy:isAlive() then
			pEnemy:setState(enemy.INACTIVE)
		end
--	ATTACK
	elseif pEnemy:getState() == enemy.ATTACK then
		ra = pEnemy:rangeAttack()
		if not pEnemy:isAlive() then
			pEnemy:setState(enemy.DYING)
		elseif pEnemy:isVisibleWideCam() and (ra  > 600) then 
			pEnemy:setState(enemy.INACTIVE)
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