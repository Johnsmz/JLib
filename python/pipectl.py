import subprocess

def pipe_open(command):
    pipe = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True); return pipe;

def pipe_readline(pipe):
    output = pipe.stdout.readline().decode().strip(); return output;

def pipe_send(pipe, string):
    pipe.stdin.write(string.encode()); pipe.stdin.write(b'\n'); pipe.stdin.flush();