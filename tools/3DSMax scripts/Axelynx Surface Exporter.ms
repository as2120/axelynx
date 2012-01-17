 rollout roll_expmesh "Axelynx Surface Export"
    (
      edittext edt_path "Path" pos:[10,10] width:300
      button btn_Browse "View" pos:[320,10] width:50
      button btn_exp "Go!" pos:[320,50] width:80 
		
      on roll_expmesh open do  
      (
        s=selection[1].name as string
        edt_path.text=(GetDir #Export)+"\\"+s+".axs"
      )
      on btn_browse pressed do
      (

        s=GetSaveFileName types:"Axelynx surface file (*.axs)|*.axs|All files (*.*)|*.*"
        if s!=undefined do
        (
          edt_path.text=s
        )
      )
  
      on btn_exp pressed do
	(

          if edt_path.text!=undefined do
			(
				f = fopen edt_path.text "wb"
				
				local export_mesh = snapshotAsMesh selection[1]
				
				local vnum=getNumVerts export_mesh
				local fnum=getNumFaces export_mesh
				
				local tvnum1 = 0
				local tvnum2 = 0
				local tfnum1 = 0
				local tfnum2 = 0
								
				if meshop.getMapSupport export_mesh 1 do
				(
				tvnum1 = meshop.getNumMapVerts export_mesh 1
				tfnum1 = meshop.getNumMapFaces export_mesh 1
				)
				
				if meshop.getMapSupport export_mesh 2  do
				(
				tvnum2 = meshop.getNumMapVerts export_mesh 2				
				tfnum2 = meshop.getNumMapFaces export_mesh 2				
				)
				
				WriteLong f vnum
				WriteLong f fnum
				
				WriteLong f tvnum1
				WriteLong f tvnum2
				
				WriteLong f tfnum1
				WriteLong f tfnum2
	
				for j=1 to fnum do
				(
					local p01=[0,0,0]
					p01=GetFace export_mesh j

					x01=(p01.x-1) as integer
					y01=(p01.y-1) as integer
					z01=(p01.z-1) as integer
					WriteLong f x01
					WriteLong f y01
					WriteLong f z01
				)
				
				for j=1 to tfnum1 do
				(
					local p01=[0,0,0]
					p01 = meshop.getMapFace export_mesh 1 j

					x01=(p01.x-1) as integer
					y01=(p01.y-1) as integer
					z01=(p01.z-1) as integer
					WriteLong f x01
					WriteLong f y01
					WriteLong f z01
				)

				for j=1 to tfnum2 do
				(
					local p01=[0,0,0]
					p01 = meshop.getMapFace export_mesh 2 j

					x01=(p01.x-1) as integer
					y01=(p01.y-1) as integer
					z01=(p01.z-1) as integer
					WriteLong f x01
					WriteLong f y01
					WriteLong f z01
				)
				
				for j=1 to tvnum1 do
				(
					local p01=[0,0,0]
					p01 = meshop.getMapVert export_mesh 1 j

					WriteFloat f p01.x
					WriteFloat f p01.y
				)

				for j=1 to tvnum2 do
				(
					local p01=[0,0,0]
					p01 = meshop.getMapVert export_mesh 2 j

					WriteFloat f p01.x
					WriteFloat f p01.y
				)
							
				for j=1 to vnum do
				(
					local p01=[0,0,0]
					p01=GetVert export_mesh j

					WriteFloat f p01.x
					WriteFloat f p01.y
					WriteFloat f p01.z
				)

				for j=1 to vnum do
				(
					local p01=[0,0,0]
					p01=GetNormal export_mesh j

					WriteFloat f p01.x
					WriteFloat f p01.y
					WriteFloat f p01.z
				)			
			)
	)
)
    if selection.count<1 then MessageBox "Select object to export please!"
    if selection.count==1 do
    (

      m_roll=newrolloutfloater "Axelynx Surface Export" 400 180

      addrollout roll_expmesh m_roll
    )
