function startLevel1CutSceneInit(pCutScene)
	pCutScene:addEntity(cutscene.SMART,1) -- 0
	pCutScene:initializeCameraEntity(2)
	pCutScene:setActivationTime(10)
end

function startLevel1CutSceneUpdate(pCutScene)
	if pCutScene:getStep() == 0 then
		if pCutScene:isEntityTrajectoryEnded(-1) then
			pCutScene:setStep(4)
		end
	elseif pCutScene:getStep() == 4 then
		pCutScene:stop()
	else
		pCutScene:stop()
	end
end