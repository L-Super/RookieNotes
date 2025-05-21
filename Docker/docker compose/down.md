`docker compose down`

停止容器并移除容器及网络

```bash
Usage:  docker compose down [OPTIONS] [SERVICES]

Stop and remove containers, networks

Options:
      --dry-run          Execute command in dry run mode
      --remove-orphans   Remove containers for services not defined in the Compose file.
      --rmi string       Remove images used by services. "local" remove only images that don't have a custom tag ("local"|"all")
  -t, --timeout int      Specify a shutdown timeout in seconds
  -v, --volumes          Remove named volumes declared in the "volumes" section of the Compose file and anonymous volumes attached to containers.
```