function startLevel1CutSceneInit(pCutScene)
	pCutScene:addEntity(cutscene.SHIP,1)
	pCutScene:initializeCameraEntity(2)
	pCutScene:setActivationTime(999)
end

function startLevel1CutSceneUpdate(pCutScene)
	if pCutScene:getStep() == 0 then
		if pCutScene:isEntityTrajectoryEnded(-1) then
			pCutScene:nextStep()
		end
	elseif pCutScene:getStep() == 1 then
		pCutScene:stop()
	end
end