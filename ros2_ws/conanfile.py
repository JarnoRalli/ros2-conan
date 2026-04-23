from conan import ConanFile


class PingPong(ConanFile):
    name = "pong_node"
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

        self.requires(f"pcl/{versions['pcl']}")
        self.requires(f"eigen/{versions['eigen']}")
        self.requires(f"nlohmann_json/{versions['nlohmann_json']}")


#    def generate(self) -> None:
#        """
#        Deploy shared libraries to the build folder.
#        """
#        dest_dir = os.path.join(self.generators_folder, "conan_libs")
#        for dep in self.dependencies.host.values():
#            if dep.cpp_info.libdirs:
#                for libdir in dep.cpp_info.libdirs:
#                    copy(self, "*.so*", libdir, dest_dir, keep_path=False)
