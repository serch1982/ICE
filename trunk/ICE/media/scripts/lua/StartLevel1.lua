function startLevel1CutSceneInit(pCutScene)
	pCutScene:initializeCameraEntity(1)
	pCutScene:addEntity(cutscene.PIGSHEEP,2) -- 0
	pCutScene:addEntity(cutscene.SHIP,3) -- 1
	
	pCutScene:setEntityLookAt(-1,0)
	
	pCutScene:setActivationTime(0.1)
end

function startLevel1CutSceneUpdate(pCutScene)

	if pCutScene:isEntityTrajectoryEnded(-1) then
		pCutScene:setStep(4)
	end

	if pCutScene:getStep() == 0 then
		if pCutScene:getCurrentTime() >= 6 then
			pCutScene:setEntityLookAt(-1,1)
			pCutScene:nextStep()
		end
	elseif pCutScene:getStep() == 1 then
		-- just wait
	elseif pCutScene:getStep() == 4 then
		pCutScene:stop()
	else
		pCutScene:stop()
	end
end