# only uncomment on the side you have your trackball on
POINTING_DEVICE_ENABLE := yes
SRC += pimoroni_trackball.c
QUANTUM_LIB_SRC += i2c_master.c
MOUSEKEY_ENABLE = yes
RGBLIGHT_ENABLE = yes
LTO_ENABLE = yes
EXTRAKEY_ENABLE = yes
DEBOUNCE_TYPE = sym_eager_pk
CONSOLE_ENABLE = no
ENCODER_ENABLE = no
