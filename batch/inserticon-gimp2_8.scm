; Copies the specified rectangle from/to the specified drawable
; This file is free software; the Free Software Foundation    
; gives unlimited permission to copy, distribute and modify it.
;
;
(define (dune-copy-rectangle dest-img dest-drawable dest-x dest-y
         img drawable x1 y1 width height)
  (gimp-rect-select img (- x1 16) y1 width height 2 0 0)
  (gimp-edit-copy drawable)
  (define floating-sel (car (gimp-edit-paste drawable TRUE)))
;  (gimp-rect-select img x1 y1 width height 2 0 0)
;  (gimp-edit-copy img drawable)
;  (define activeLayer (car (gimp-image-get-active-layer img)))
;   (define activeLayer (car (gimp-floating-sel-to-layer floating-sel)))
;   (gimp-selection-none img)
   (gimp-layer-set-offsets floating-sel dest-x dest-y)   
;  (gimp-floating-sel-anchor floating-sel)   
;  (set! floating-sel (car (gimp-edit-paste dest-drawable TRUE)))
;  (gimp-floating-sel-anchor floating-sel)   
)

(define (script-fu-new-duneInsertIcon-batch inFilename inPosition)

 (define iconImage (car (gimp-file-load 1 inFilename inFilename))) 

 (define iconWidth (car (gimp-image-width iconImage)))

 (define iconHeight (car (gimp-image-height iconImage)))

 ; create new layer in image
 (define theLayer (car (gimp-layer-new iconImage iconWidth iconHeight 
                                     0 "layer 1" 100 0) ) ) 

 (define theImageWidth (+ iconWidth 16))

 (define theImageHeight iconHeight)

 (define theImage iconImage)

; (plug-in-icc-profile-apply-rgb RUN-NONINTERACTIVE theImage 0 0)
;
; (gimp-image-add-layer theImage theLayer 0)

 (gimp-image-resize theImage theImageWidth theImageHeight 0 0) 

 (define saveLayer (car (gimp-image-flatten theImage)))
 
 (gimp-convert-rgb iconImage)
 
 (file-bmp-save 1 theImage saveLayer inFilename inFilename)

 (set! iconImage (car (gimp-file-load 1 inFilename inFilename))) 

 (set! theLayer (car (gimp-layer-new iconImage (+ 16 iconWidth) iconHeight 
                                     1 "layer 1" 100 0) ) ) 

 (gimp-image-add-layer iconImage theLayer 0)

 (define sourceLayer (car (gimp-layer-new iconImage (+ 16 iconWidth) iconHeight 
                                     1 "layer 1" 100 0) ) ) 

 (gimp-image-add-layer iconImage sourceLayer 0)


 (define position (- (* 16 inPosition) 16))

 (define width (- iconWidth position))

 (dune-copy-rectangle theImage sourceLayer position 0
                      theImage theLayer (+ 16 position) 0 width iconHeight)

 (gimp-convert-indexed theImage 0 0 16 FALSE FALSE "")
 
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
