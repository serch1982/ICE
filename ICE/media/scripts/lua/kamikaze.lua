-- Function for Kamikaze Enemy Type
-- param: actual state
-- return: new state
function KamikazeLogic( pEnemy )
	if pEnemy:getState() == enemy.STOPPED then
		if pEnemy:isActive() then
			pEnemy:setState(enemy.ATTACK)
		elseif not pEnemy:isAlive() then
			pEnemy:setState(enemy.INACTIVE)
		end
	elseif pEnemy:getState() == enemy.ATTACK then
		if not pEnemy:isAlive() then
			pEnemy:setState(enemy.DYING)
		end
	elseif pEnemy:getState() == enemy.DYING then
		if pEnemy:isAnimDyingEnded() then
			pEnemy:setState(enemy.INACTIVE)
		end
	end
	return pEnemy:getState()
end