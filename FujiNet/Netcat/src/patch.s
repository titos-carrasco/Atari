; ca65 V2.18 en Debian reporta:
;   Unresolved external 'c_sp' referenced

; este patch permite corregir esto
.importzp sp
.exportzp c_sp := sp
