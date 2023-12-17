def cuenta_gotas_diario(segundos_por_gota):
    gotas_por_ml=20
    segundos_por_dia = 86400
    return segundos_por_dia/(segundos_por_gota * gotas_por_ml)

def ritmo_gotas_por_ml_diario(ml):
    gotas_por_ml=20
    segundos_por_dia = 86400
    
    gotas_por_ml_deseados = gotas_por_ml * ml

    ritmo_diario = segundos_por_dia / gotas_por_ml_deseados
    return ritmo_diario



    
# 20g - 1ml
# 20000g x- 1000ml 

# 86400 - 20000
# x - 1