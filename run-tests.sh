pytest pytest_hello_world.py \
    --target esp32 \
    --embedded-services=qemu,idf \
    --qemu-image-path=/workspaces/ESP32-indi-led-strip-controller/build/merged_qemu.bin