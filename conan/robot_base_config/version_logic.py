import os
from typing import Dict


class RobotVersions:
    """
    Centralized version management for robot dependencies.

    This class acts as a single source of truth for library versions,
    ensuring that C++ libraries managed by Conan align with the
    active ROS 2 distribution to prevent binary ABI mismatches.
    """

    @staticmethod
    def get_versions() -> Dict[str, str]:
        """
        Retrieve dependency versions based on the ROS_DISTRO environment variable.

        The method detects the active ROS distribution (e.g., 'humble', 'kilted')
        to select compatible library versions. This ensures that all developers
        and CI runners are pinned to the same version of a dependency.

        Returns
        -------
        versions : Dict[str, str]
            A dictionary where keys are Conan package names and values
            are specific version strings (e.g., {"pcl": "1.14.1"}).

        Examples
        --------
        >>> versions = RobotVersions.get_versions()
        >>> print(versions["pcl"])
        1.14.1
        """
        ros_distro: str = os.getenv("ROS_DISTRO", "humble")

        # We pin versions strictly to ensure reproducibility across the fleet.
        # Future distros can be added here as the robot stack evolves.
        if ros_distro == "kilted":
            return {"eigen": "3.4.0", "yaml-cpp": "0.8.0", "nlohmann_json": "3.11.3"}

        if ros_distro == "humble":
            return {"eigen": "3.4.0", "yaml-cpp": "0.8.0", "nlohmann_json": "3.11.3"}

        # Default fallback for rolling or unknown distributions
        return {"eigen": "3.4.0", "yaml-cpp": "0.8.0", "nlohmann_json": "3.11.3"}
