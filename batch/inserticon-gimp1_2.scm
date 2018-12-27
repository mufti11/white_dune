 ; Copies the specified rectangle from/to the specified drawable
; This file is free software; the Free Software Foundation    
; gives unlimited permission to copy, distribute and modify it.
;
;
(define (dune-copy-rectangle dest-img dest-drawable dest-x dest-y
         img drawable x1 y1 width height)
  (gimp-rect-select img x1 y1 width height REPLACE FALSE 0)
  (gimp-edit-copy img drawable)
  (let ((floating-sel (car (gimp-edit-paste dest-img dest-drawable FALSE))))
    (gimp-layer-set-offsets floating-sel dest-x dest-y)   
    (gimp-floating-sel-anchor floating-sel))   
  (gimp-selection-none img))

(define (script-fu-new-duneInsertIcon-batch inFilename inPosition)

 (set! iconImage (car (gimp-file-load 1 inFilename inFilename))) 

 (set! iconWidth (car (gimp-image-width iconImage)))

 (set! iconHeight (car (gimp-image-height iconImage)))

 ; create new layer in image
 (set! theLayer (car (gimp-layer-new iconImage iconWidth iconHeight 
                 RGBA_IMAGE "layer 1" 100 NORMAL) ) ) 

 (set! theImageWidth (+ iconWidth 16))

 (set! theImageHeight iconHeight)

 (set! theImage iconImage)

; (gimp-image-add-layer theImage theLayer 0) 

; (gimp-palette-set-background '(255 255 255) ) 
; (gimp-palette-set-foreground '(0 0 0)) 
; (gimp-edit-fill theImage theLayer)

 (gimp-image-resize theImage theImageWidth theImageHeight 0 0) 
 (gimp-layer-resize theLayer theImageWidth theImageHeight 0 0) 

 (set! saveLayer (car (gimp-image-flatten theImage)))
 
 (file-bmp-save 1 theImage saveLayer inFilename inFilename)

 (set! iconImage (car (gimp-file-load 1 inFilename inFilename))) 

 (set! theLayer (car (gimp-layer-new iconImage (+ 16 iconWidth) iconHeight 
                 RGBA_IMAGE "layer 1" 100 NORMAL) ) ) 

 (set! position (- (* 16 inPosition) 16))

 (set! width (- iconWidth position))

 (gimp-rect-select theImage position 0 width iconHeight ADD FALSE 0)

; (dune-copy-rectangle theImage theLayer position 0
;                      theImage theLayer (+ 16 position) 0 width iconHeight)

 (gimp-display-new theImage)

) 

(script-fu-register 
  "script-fu-new-duneInsertIcon-batch" 
  "<Toolbox>/Xtns/Script-Fu/my/NewInsertIconbatch" 
  "copy a icon file and include a new icon" 
  "copyright 2004, J. \"MUFTI\" Scheurich, Gnu Public License (GPL)" 
  "MUFTI"
  "2004" 
  "" 
  SF-FILENAME "Filename:"    "node_icons.bmp" 
  SF-VALUE "Position to include new icon" "4" 
) 
