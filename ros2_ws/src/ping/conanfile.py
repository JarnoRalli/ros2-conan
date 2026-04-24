from conan import ConanFile


class PingNode(ConanFile):
    name = "ping_node"
    version = "1.0.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    python_requires = "robot_base_config/1.0@einherjar/stable"

    def requirements(self) -> None:
        """
        Define requirements by pulling pinned versions from the central config.
        """
        versions = self.python_requires[
            "robot_base_config"
        ].module.RobotVersions.get_versions()

        self.requires(f"yaml-cpp/{versions['yaml-cpp']}")
        self.requires(f"eigen/{versions['eigen']}")
        self.requires(f"nlohmann_json/{versions['nlohmann_json']}")
