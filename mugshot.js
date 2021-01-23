//npm install imgur -s
var imgur = require('imgur');

// https://api.imgur.com/oauth2/addclient
// Create an account and set type to Anonymous
// Set callback to: https://www.getpostman.com/oauth2/callback
// Youll get a Client ID - enter it below

const CLIENT_ID = '';
mp.events.add("uploadPlayerHeadshot", (player, image) => {
    console.log(player, image);
    imgur.setClientId(CLIENT_ID);
    imgur.uploadBase64(image.replace("data:image/png;base64,", ""))
        .then(function (json) {
            console.log(json.data.link);
        })
        .catch(function (err) {
            console.error(err.message);
        });
})
