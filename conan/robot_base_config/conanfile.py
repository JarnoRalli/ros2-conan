from conan import ConanFile


class RobotBaseConfig(ConanFile):
    name = "robot_base_config"
    version = "1.0"

    # This tells Conan which files to bring from your disk into the recipe
    exports = "version_logic.py"


# Import the class into the global namespace of this file
