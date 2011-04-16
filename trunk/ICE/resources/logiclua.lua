function EnemyLogicState(enemy,iceLogicLua)
	--example calling iceEnemy method
	if (enemy:getState() == iceEnemy.FOLLOWING_TRAJECTORY) then
		-- example calling iceLogicLua method
		if(iceLogicLua:enemyIsVisible() == true) then 
			return iceEnemy.GOING_TO_PLAYER
		else 
			return iceEnemy.FOLLOWING_TRAJECTORY
		end
	else
		return  enemy:getState()
	end
end
