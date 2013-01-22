add_avr_executable(
    robogram
    main.c
    settings.h
    bluetooth.c bluetooth.h
    eyes.c eyes.h
    io.c io.h
    motors.c motors.h
    timers.c timers.h
    uart.c uart.h
    sensors.c sensors.h
)
