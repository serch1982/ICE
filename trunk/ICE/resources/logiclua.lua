function EnemyLogicState(enemy, evtTime)
	--get the current state
	state = enemy:getState()
	--get range between the enemy and the player 
	range = enemy:rangeAttack()

	--To activate the enemies
	if (state == iceEnemy.INACTIVE) then 
		if (enemy:checkActivationTime(evtTime) == true) then 
			enemy:activate()
			enemy:setState(iceEnemy.FOLLOWING_TRAJECTORY)
		end
	end
	--states to MINIMAGMATON
	minRange = 10
	maxRange = 500
	if (enemy:getType() == iceEnemy.MINIMAGMATON) then 
		if (state == iceEnemy.FOLLOWING_TRAJECTORY) then 
			if(enemy:isVisiblePlayerCam()) then
				if (range < maxRange) and (range > minRange) then 
					enemy:setState(iceEnemy.ATTACKING)
				else
					if(range < 0) then 
						enemy:setState(iceEnemy.DEAD)
					end
				end
			end
			if not(enemy:isVisibleWideCam())then
					enemy:setState(iceEnemy.DEAD)
			end
		else
			if(state == iceEnemy.ATTACKING) then
				if(enemy:isVisiblePlayerCam()) then 
					if(range > maxRange)then 
						enemy:setState(iceEnemy.FOLLOWING_TRAJECTORY)
					end
					if(range < 0) then 
						enemy:setState(iceEnemy.DEAD)
					end
				else
					enemy:setState(iceEnemy.FOLLOWING_TRAJECTORY)
				end
				if not(enemy:isVisibleWideCam())then
					enemy:setState(iceEnemy.DEAD)
				end
			end 
		end
	end 

	--states to KAMIKAZE
	if (enemy:getType() == iceEnemy.KAMIKAZE) then 
		if (state == iceEnemy.FOLLOWING_TRAJECTORY) then 
			if not(enemy:isVisibleWideCam()) then
				enemy:setState(iceEnemy.DEAD)
			end
		end		
	end 

	--states to SMART
	minRange = 10
	maxRange = 600
	if (enemy:getType() == iceEnemy.SMART) then 
		if (state == iceEnemy.FOLLOWING_TRAJECTORY) then 
			if(enemy:isVisiblePlayerCam()) then
				if (range < maxRange) and (range > minRange) then 
					enemy:setState(iceEnemy.ATTACKING)
				else
					if(range < 0) then 
						enemy:setState(iceEnemy.DEAD)
					end
				end
			end
			if not(enemy:isVisibleWideCam())then
					enemy:setState(iceEnemy.DEAD)
			end
		else
			if(state == iceEnemy.ATTACKING) then
				if(enemy:isVisibleWideCam()) then 
					if(range > maxRange)then 
						enemy:setState(iceEnemy.FOLLOWING_TRAJECTORY)
					end
					if(range < 0) then 
						enemy:setState(iceEnemy.DEAD)
					end
				else
					enemy:setState(iceEnemy.FOLLOWING_TRAJECTORY)
				end
				if not(enemy:isVisibleWideCam())then
					enemy:setState(iceEnemy.DEAD)
				end
			end 
		end
	end 
end
