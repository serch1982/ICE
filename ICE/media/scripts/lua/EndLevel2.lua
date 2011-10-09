function endLevel2CutSceneInit(pCutScene)
	pCutScene:initializeCameraEntity(1)
	pCutScene:addEntity(cutscene.SHIP,2) -- 0
	pCutScene:addEntity(cutscene.MAGMATON,3) -- 1
	pCutScene:addEntity(cutscene.DUMMY,4) -- 2
	
	pCutScene:setEntityLookAt(-1,2)
	pCutScene:setEntityLookAt(0,2)
	
	pCutScene:setActivationTime(95)
end

function endLevel2CutSceneUpdate(pCutScene)

	if pCutScene:isEntityTrajectoryEnded(-1) then
		pCutScene:setStep(13)
	end

	if pCutScene:getStep() == 0 then
	
		pCutScene:stopAllSounds()
		pCutScene:playSound(26,1)

		pCutScene:stopEntity(-1)
		pCutScene:nextStep()
		pCutScene:startEntityAnimation(1,"CutSceneFinal_Clip")
		
	elseif pCutScene:getStep() == 1 then -- comienza salida -- rugidito
		if pCutScene:getCurrentTime() >= 28.7 then
			pCutScene:startEntityAnimation(0,"enfado2_Clip")
			pCutScene:showBlur()
			pCutScene:playSound(28,0.3)
			pCutScene:nextStep()
		end
	elseif pCutScene:getStep() == 2 then -- acaba salida
		if pCutScene:getCurrentTime() >= 29.4 then
			pCutScene:hideBlur()
			pCutScene:nextStep()
		end	
	elseif pCutScene:getStep() == 3 then -- comienza golpe a dos manos
		if pCutScene:getCurrentTime() >= 36.12 then
			pCutScene:startEntityAnimation(0,"impacto4_Clip")
			pCutScene:showBlur()
			pCutScene:nextStep()
		end
	elseif pCutScene:getStep() == 4 then -- acaba golpe a dos manos
		if pCutScene:getCurrentTime() >= 36.56 then
			pCutScene:hideBlur()
			pCutScene:nextStep()
		end
	elseif pCutScene:getStep() == 5 then -- comienza acercamiento
		if pCutScene:getCurrentTime() >= 38.44 then
			pCutScene:showBlur()
			pCutScene:nextStep()
		end
	elseif pCutScene:getStep() == 6 then -- acaba acercamiento
		if pCutScene:getCurrentTime() >= 38.84 then
			pCutScene:hideBlur()
			pCutScene:nextStep()
		end
	elseif pCutScene:getStep() == 7 then -- rugido
		if pCutScene:getCurrentTime() >= 40.8 then
			pCutScene:showBlur()
			pCutScene:playSound(27,0.4)
			pCutScene:nextStep()
		end
	elseif pCutScene:getStep() == 8 then -- fin rugido
		if pCutScene:getCurrentTime() >= 43.4 then
			pCutScene:hideBlur()
			pCutScene:nextStep()
		end
	elseif pCutScene:getStep() == 9 then -- inicio giro
		if pCutScene:getCurrentTime() >= 43.64 then
			pCutScene:startEntity(-1)
			pCutScene:nextStep()
		end
	elseif pCutScene:getStep() == 10 then -- fin giro
		if pCutScene:getCurrentTime() >= 54.3 then
			pCutScene:stopEntity(-1)
			pCutScene:showToBeContinued()
			pCutScene:nextStep()
		end
	elseif pCutScene:getStep() == 11 then
		if pCutScene:getCurrentTime() >= 60 then
			pCutScene:nextStep()
		end
	elseif pCutScene:getStep() == 12 then
		pCutScene:stop()
	else
		pCutScene:stop()
	end
end