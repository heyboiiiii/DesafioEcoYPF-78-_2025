Receptor de telemetria en boxes(LORA).

1- Se recibira por el modulo SMT1278, la siguiente informacion del automovil:

    Tension y Corriente de baterias.
    Velocidad.
    Posicionamiento(Latitud & Longitud).
    Estado de pulsadores del volante.
    
2- Esta informacion es mostrada atraves de una pagina en un servidor interno montado por el esp32 con littleFS en una red local generada por el modulo WiFi del propio microcontrolador.

*Codigo de pagina web mas a detalle en directorio apps:

