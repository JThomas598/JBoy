SECTION "Header", ROM0[$100]
    jp EntryPoint

    ds $150 - @, 0 ; Make room for the header

EntryPoint:
    ld A, $FF
    add A, $01
    ld A, $0F
    adc 0
    jp Done
    
func1:
    rra
    call func2
    ret

func2:
    rla
    ret
failure:
    ld b, $FF


Done:
    jp Done