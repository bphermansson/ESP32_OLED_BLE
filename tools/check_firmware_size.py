import os

Import("env")


def check_firmware_size(source, target, env):
    firmware = env.subst("$BUILD_DIR/${PROGNAME}.bin")
    size = os.path.getsize(firmware)
    maximum = int(env.BoardConfig().get("upload.maximum_size"))
    if size > maximum:
        raise RuntimeError(
            "Firmware binary is {} bytes, exceeding the app partition by {} bytes "
            "(maximum {} bytes).".format(size, size - maximum, maximum)
        )
    print("Firmware binary: {} / {} bytes".format(size, maximum))


env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", check_firmware_size)
env.AddPreAction("upload", check_firmware_size)
