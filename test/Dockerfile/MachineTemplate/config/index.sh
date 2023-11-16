## LoadLinker Test Template ##
#!/bin/sh

NAME=$1
VERSION=$2
IP_ADDRESS=$3

cat << EOF > /usr/share/nginx/html/index.html
<html>
<body>
<div class="default">
    <center>
        <h1>Welcome to LoadLinker!</h1>
        <h2>$NAME $VERSION</h2>
        <h3>$IP_ADDRESS<h3>
    </center>
</div>
</center>
</body>
</html>

<style>
    .default {
        position: absolute;
        top: 50%;
        left: 50%;
        margin-top: -100px;
        margin-left: -200px;
    }
</style>
EOF