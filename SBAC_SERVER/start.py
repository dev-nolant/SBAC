import subprocess
from security import safe_command

print("?IMPORTANT? You're about to start a new instance of SBAC_SERVER")

class createInstance:
    def __init__(self, params):
        # Cleaning up and preparing the parameters
        cleaned = [s.replace(" ", "") for s in params if s.strip()]
        self.params = [*cleaned]

    # Returns selected params
    def getParams(self):
        return self.params

    # Method to start a new process with the given parameters
    def startProcess(self, script_name):
        command = ["python3", script_name] + self.params
        print("Starting process with command:", ' '.join(command))
        safe_command.run(subprocess.Popen, command)


ins = createInstance(input("? params: ").split() )

ins.startProcess("sbac_server.py")
