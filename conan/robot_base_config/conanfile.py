from version_logic import RobotVersions  # noqa: F401

from conan import ConanFile


class RobotBaseConfig(ConanFile):
    name = "robot_base_config"
    version = "1.0"

    # This tells Conan which files to bring from your disk into the recipe
    exports = "version_logic.py"
