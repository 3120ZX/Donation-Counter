const LocalStrategy = require('passport-local').Strategy
const bcrypt = require('bcrypt')
const Admin = require('./models/admin')


function initialize(passport){
    const authenticateAdmin = async (username,password, done) => {
        Admin.findOne({ username: username }, function (err, user) {
            if (err) return done(err);
            if (!user) return done(null, false, { message: 'Incorrect username / password.' });
    
            bcrypt.compare(password, user.password, function (err, res) {
                if (err) return done(err);
                if (res === false) return done(null, false, { message: 'Incorrect username / password.' });
                
                return done(null, user);
            });
        })}

    passport.use(new LocalStrategy({usernameField: 'username'},authenticateAdmin))
    passport.serializeUser((user,done) => done(null, user.id))
    passport.deserializeUser((id,done) => {
        Admin.findById(id, function (err, user) {
            done(err, user);
        });
    })
}

module.exports = initialize