build:
    pio -f -c vim run

upload:
    pio -f -c vim run --target upload
    pio -f -c vim run --target monitor