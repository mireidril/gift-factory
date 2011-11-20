filename = GetSaveFileName caption "GiftFactory scene file"

if filename != undefined then 
(
	
	-- Retrieve the whole objects selection
	listSelection = getCurrentSelection()
	
	file = createFile filename


	for i=1 to listSelection.count do
	(
		obj = listSelection[i]
		
		/* Extract infos from the mesh name and write them in the SET file
			- its instance name
			- its base mesh index
			-its material
		*/
	/*format "% % % " obj.ArrowsObject.objName obj.ArrowsObject.objID obj.ArrowsObject.materialFile to:file*/
		
		-- Write the global matrix
		--m = obj.objecttransform
		--s = obj.scale
		--pos = obj.pos
		
		format "% " obj.name to:file
		-- Change axis : Z becomes Y and Y becomes -Z
		rot = quat obj.rotation.x obj.rotation.z -obj.rotation.y obj.rotation.w
		m = rot as Matrix3
		m.row4 = obj.pos
		
		format "% % % % " m[1][1]	m[1][2] 	m[1][3]	m[4][1] to:file
		format "% % % % " m[2][1] 	m[2][2]	m[2][3]	m[4][3] to:file
		format "% % % % " m[3][1] 	m[3][2] 	m[3][3]	-m[4][2] to:file		
		format "0.0 0.0 0.0 1.0" to:file
		
		/*
		m = obj.rotation as Matrix3
		m.row4 = obj.pos		
		
		
		-- Change axis : Z becomes Y and Y becomes -Z
		format "% % % % " m[1][1]	m[1][3] 	m[1][2]	m[4][1] to:file
		format "% % % % " m[3][1] 	m[3][3]	m[3][2]	m[4][3] to:file
		format "% % % % " m[2][1] 	m[2][3] 	m[2][2]	-m[4][2] to:file		
		format "0.0 0.0 0.0 1.0" to:file
		*/
		if i < listSelection.count then format "\n" to:file
	
	)

	fclose file
)
