import subprocess

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
        subprocess.Popen(command)


# Simulating user input for demonstration; replace 'input("? params: ")' with your actual input handling
ins = createInstance(input("? params: ").split() )

# Assuming 'app.py' is the script you want to run; replace it with your actual script name
ins.startProcess("sbac_server.py")
