obj-$(CONFIG_ONEWIRE_GPIO) += onewire_gpio.o
obj-$(CONFIG_BATT_VERIFY_BY_DS28E16) += onewire_gpio.o ucl_sha3.o sha384_software.o ds28e16.o

# 24.0 kernel turns this into an error; these casts are intentional (from the 23.0 vendor code)
ccflags-y += -Wno-void-pointer-to-int-cast -Wno-pointer-to-int-cast
