const mongo = require('mongodb');
const bodyParser= require('body-parser');
const express = require('express');
const path = require('path');

// const sjocker = require("./public/")

const app = express();
const port = 3000;

const MongoClient = require('mongodb').MongoClient;
const uri = "mongodb+srv://A01654203:ql8DjfgC6B03EXba@cluster0.teqbbtq.mongodb.net/?retryWrites=true&w=majority";
const client = new MongoClient(uri, { useNewUrlParser: true, useUnifiedTopology: true });


app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static(__dirname + '/public'));
app.use(express.static(__dirname + '/Img'));

function hello(user)
{
  elem = user
  console.log("From hello")
  console.log(elem)
  module.exports = {elem}
}

app.get('/', (req, res) => {
  res.sendFile(__dirname + '/public/index.html');
});

//TODO login
app.get('/login', (req, res) => {
  res.sendFile(__dirname + '/public/login.html');
});

app.post('/login', (req, res) => {

  client.connect(err => {
    const collection = client.db("Equinex").collection("User");
    collection.findOne({ username: req.body.username}, function(err, user) {
      
      //document.getElementById(saludo).user

      // In case the user not found   
      if(!user)
      {
        console.log("[-] Invalid user")
        
        return res.sendFile(__dirname + '/public/errorpage.html')
      }

      console.log('[+] User found...');
      console.log(user)

      if(err) {
        console.log(err);
        console.log("[-] Invalid user or null")
        // client.close();
        return res.sendFile(__dirname + '/public/errorpage.html');
      } 
      if (user.password === req.body.password){
        if (user.username === "super@admin" && user.password === "admin")
        {
          console.log('[+] Admin detected - all hail the super user'); 
          return res.sendFile(__dirname + '/public/mainpageadmin.html')
        }
        else
        {
          console.log('[+] User and password are correct...');

          // client.close();
          return res.sendFile(__dirname + '/public/mainpage.html');
        }
      } else {
        console.log("[-] Wrong credentials...");
        // client.close();
        return res.sendFile(__dirname + '/public/errorpage.html');
      }              
    });    
  });
});

// admin page
app.get('/mainpageadmin', (req, res) => {
  res.sendFile(__dirname + '/public/mainpageadmin.html');
});

app.get('/registerjockey', (req, res) => 
{
  res.sendFile(__dirname + '/public/registerjockey.html'); 
});

app.get('/deletejockey', (req, res) => 
{
  res.sendFile(__dirname + '/public/deletejockey.html');
});

app.get('/updatejockey', (req, res) =>
{
  res.sendFile(__dirname + '/public/updatejockey.html');
});

app.get('/mainpage', (req, res) => {
  res.sendFile(__dirname + '/public/mainpage.html');
});
  // res.send(`Username: ${username} Password: ${password}`);

//TODO Find by given name
app.post('/findjockey', function(req, res) {
  client.connect(err => {
    var collection;
    var us;

    console.log (":(")
    console.log(req.body.nomjin);
    collection = client.db("Equinex").collection("User");
    collection.findOne({ username: req.body.nomjin}, function(err, user) {

      if(user == null) {
        console.log("Invalid user :(")
        return res.sendFile(__dirname + '/public/noFoundH.html')
      }

      if (err) {
        return res.sendFile("/public/errorpage.html");
      }

      if(user){
        // console.log(user.username)
        // hello(user)
        console.log(user)
        console.log("success...");
        return res.sendFile(__dirname + '/public/foundJ.html')
      }
      // client.close();
      // console.log("nah");
    });
  });
});

//TODO Find by given name
app.post('/findjockeyadmin', function(req, res) {
  client.connect(err => {
    var collection;
    var us;

    console.log (":(")
    console.log(req.body.nomjin);
    collection = client.db("Equinex").collection("User");
    collection.findOne({ username: req.body.nomjin}, function(err, user) {

      if(user == null) {
        console.log("Invalid user :(")
        return res.sendFile(__dirname + '/public/noFoundH.html')
      }

      if (err) {
        return res.sendFile("/public/errorpage.html");
      }

      if(user){
        // console.log(user.username)
        // hello(user)
        console.log(user)
        console.log("success...");
        return res.sendFile(__dirname + '/public/foundJadmin.html')
      }
      // client.close();
      // console.log("nah");
    });
  });
});


app.post('/findhorsey', function(req, res) {
  client.connect(err => {
    var collection;
    var us;

    console.log (":(")
    console.log(req.body.nomhor);
    collection = client.db("Equinex").collection("Caballos");
    collection.findOne({ name: req.body.nomhor}, function(err, user) {

      if(user == null) {
        console.log("Invalid user :(")
        return res.sendFile(__dirname + '/public/noFoundH.html')
      }

      if (err) {
        return res.sendFile("/public/noFoundH.html");
      }

      if(user){
        return res.sendFile(__dirname + '/public/foundhorse.html');
        console.log(user.name)
        console.log("success...");
      }
      // client.close();
      // console.log("nah");
    });
  });
});
app.post('/findhorseyadmin', function(req, res) {
  client.connect(err => {
    var collection;
    var us;

    console.log (":(")
    console.log(req.body.nomhor);
    collection = client.db("Equinex").collection("Caballos");
    collection.findOne({ name: req.body.nomhor}, function(err, user) {

      if(user == null) {
        console.log("Invalid user :(")
        return res.sendFile(__dirname + '/public/noFoundH.html')
      }

      if (err) {
        return res.sendFile("/public/noFoundH.html");
      }

      if(user){
        return res.sendFile(__dirname + '/public/foundhorseadmin.html');
        console.log(user.name)
        console.log("success...");
      }
      // client.close();
      // console.log("nah");
    });
  });
});
//register directions
app.get('/register', (res) => {
  res.sendFile(__dirname + '/public/register.html');
})
app.post('/register', (req, res) => {
  client.connect(err => {
    const collection = client.db("Equinex").collection("User");
    collection.insertOne(req.body, function(err, res) {
      if(err) {
        console.log('[-] User not found...');
        // client.close();
        return res.sendFile(__dirname + '/public/errorpage.html');
      } 
      console.log("1 document inserted");
      // client.close();
    });
    console.log("success...");
  });
  return res.sendFile(__dirname + '/public/success.html');
});
app.post('/registeradmin', (req, res) => {
  client.connect(err => {
    const collection = client.db("Equinex").collection("User");
    collection.insertOne(req.body, function(err, res) {
      if(err) {
        console.log('[-] User not found...');
        // client.close();
        return res.sendFile(__dirname + '/public/errorpage.html');
      } 
      console.log("1 document inserted");
      // client.close();
    });
    console.log("success...");
  });
  return res.sendFile(__dirname + '/public/successadmin.html');
});
//register caballos to specific collections
app.get('/registercaballo', (res) => {
  res.sendFile(__dirname + '/public/registercaballo.html');
})

app.get('/registercaballoadmin.html', (req,res) =>
{
  res.sendFile(__dirname + '/public/registercaballoadmin.html');
});

app.post('/registercaballoadmin', (req, res) => {
  client.connect(err => {
    const collection = client.db("Equinex").collection("Caballos");
    collection.insertOne(req.body, function(err, res) {
      if (err) throw err;
      console.log("[+] Horse registered...");
      
      // client.close();
    });
    console.log("success...");
  });
  res.sendFile(__dirname + '/public/mainpageadmin.html');
});

app.post('/registercaballo', (req, res) => {
  client.connect(err => {
    const collection = client.db("Equinex").collection("Caballos");
    collection.insertOne(req.body, function(err, res) {
      if (err) throw err;
      console.log("[+] Horse registered...");
      
      // client.close();
    });
    console.log("success...");
  });
  res.sendFile(__dirname + '/public/mainpage.html');
});

app.post('/deletejockey', (req, res) => {
  // var myquery = { address: 'Mountain 21' };
  client.connect(err => {
    const collection = client.db("Equinex").collection("User");
    collection.deleteOne(req.body, function(err, res) {
      if(err) {
        // client.close();
        return res.sendFile(__dirname + '/public/errorpage.html');
      } 
      console.log("[+] Jockey deleted...");
      // client.close();
    });
    console.log("success...");
  });
  res.sendFile(__dirname + '/public/mainpageadmin.html');
});

app.post('/deletecaballoadmin', (req, res) => {
  // var myquery = { address: 'Mountain 21' };
  client.connect(err => {
    const collection = client.db("Equinex").collection("Caballos");
    collection.deleteOne(req.body, function(err, res) {
      if(err) {
        // client.close();
        return res.sendFile(__dirname + '/public/errorpage.html');
      } 
      console.log("[+] Horse register deleted...");
      // client.close();
    });
    console.log("success...");
  });
  res.sendFile(__dirname + '/public/mainpageadmin.html');
});

//delete caballo from collection
app.post('/deletecaballo', (req, res) => {
  // var myquery = { address: 'Mountain 21' };
  client.connect(err => {
    const collection = client.db("Equinex").collection("Caballos");
    collection.deleteOne(req.body, function(err, res) {
      if(err) {
        // client.close();
        return res.sendFile(__dirname + '/public/errorpage.html');
      } 
      console.log("[+] Horse register deleted...");
      // client.close();
    });
    console.log("success...");
  });
  res.sendFile(__dirname + '/public/mainpage.html');
});

app.get('/searchjockey', (req, res) =>
{
  res.sendFile(__dirname + '/public/searchjokey.html')
});
app.get('/searchhorse', (req, res) =>
{
  res.sendFile(__dirname + '/public/searchhorse.html')
});
app.get('/updatecaballo', (req,res) =>
{
  res.sendFile(__dirname + '/public/updatecaballo.html')
});

// update value of a horse 
app.post('/updatecaballo', (req, res) => {
  // var myquery = { address: 'Mountain 21' };
  client.connect(err => {
    const collection = client.db("Equinex").collection("Caballos");
    collection.updateOne({ horseID: req.body.horseID}, { $set: {name: req.body.name} },function(err, res) {
      if(err) {
        // client.close();
        res.sendFile(__dirname + '/public/errorpage.html');
      } 
      console.log("[+] Horse information updated...");
      // client.close();
    });
    console.log("success...");
  });
  res.sendFile(__dirname + '/public/mainpage.html');
});

// update value of a horse 
app.post('/updatecaballoadmin', (req, res) => {
  // var myquery = { address: 'Mountain 21' };
  client.connect(err => {
    const collection = client.db("Equinex").collection("Caballos");
    collection.updateOne({ horseID: req.body.horseID}, { $set: {name: req.body.name} },function(err, res) {
      if(err) {
        // client.close();
        res.sendFile(__dirname + '/public/errorpage.html');
      } 
      console.log("[+] Horse information updated...");
      // client.close();
    });
    console.log("success...");
  });
  res.sendFile(__dirname + '/public/mainpageadmin.html');
});

// update value of a horse 
app.post('/updatejockeyadmin', (req, res) => {
  // var myquery = { address: 'Mountain 21' };
  client.connect(err => {
    const collection = client.db("Equinex").collection("User");
    collection.updateOne({ username: req.body.username}, { $set: {name: req.body.name} },function(err, res) {
      if(err) {
        // client.close();
        res.sendFile(__dirname + '/public/errorpage.html');
      } 
      console.log("[+] Horse information updated...");
      // client.close();
    });
    console.log("success...");
  });
  res.sendFile(__dirname + '/public/mainpageadmin.html');
});


app.listen(port, () => {
  console.log(`Example app listening at http://localhost:${port}`);
});