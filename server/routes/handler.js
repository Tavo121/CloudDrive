const express = require('express');
const router = express.Router();

router.get('/tweets', (req, res) =>{
    //info que se presenta en la pagina
});

router.post('/addTweet', (req, res) =>{
    res.end('NA');
});

module.exports = router;