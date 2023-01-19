from .nrp_server_launchers import NRPCoreServerLauncher
from nrp_core.docker_handle import NRPDockerHandle

class NRPCoreDockerLauncher(NRPCoreServerLauncher):

    def __init__(self, args, experiment_folder, docker_daemon_ip, image_name, log_file, get_archives):
        proc_cmd = 'nrp-run.bash xvfb-run NRPCoreSim ' + ' '.join(args)

        self._log_file = log_file
        self._get_archives = get_archives
        self._docker = NRPDockerHandle(docker_daemon_ip, image_name, experiment_folder, proc_cmd, [])

    def is_alive_nrp_process(self) -> bool:
        status = self._docker.get_status()
        return status and status['running']

    def get_exit_report(self) -> dict:
        status = self._docker.get_status()
        return {'exit_code': status['exit_code'] if status else None,
                'logs': self._docker.get_logs()}

    def kill_nrp_process(self) -> None:
        status = self._docker.get_status()
        if status:
            self._docker.get_experiment_archive(self._log_file)
            for archive in self._get_archives:
                self._docker.get_experiment_archive(archive)

            self._docker.stop()
            self._docker.remove()
