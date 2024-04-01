from flask import Flask, request, jsonify, render_template
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
import argparse

# Setting up argparse to handle command-line arguments
parser = argparse.ArgumentParser(description="Start a new instance of SBAC (Sandbox Anti-Cheat) Server.")
parser.add_argument("--debug", default="True", action="store_true", help="Enable debug mode.")
parser.add_argument("--timeout", type=int, default=600, help="Set timeout in seconds. Default is 600 seconds (10 minutes).")
parser.add_argument("--port", type=int, default=8080, help="Set port number. Default is 8080.")
parser.add_argument("--host", default="0.0.0.0", help="Set host. Default is '0.0.0.0'.")
args, unknown = parser.parse_known_args()  # Ensure the Flask reloader doesn't duplicate arguments

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///sbac.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)

# Database model for detections
class Detection(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    ip = db.Column(db.String, nullable=False)
    username = db.Column(db.String, nullable=False)
    description = db.Column(db.String, nullable=False)
    recorded_time = db.Column(db.DateTime, default=datetime.utcnow)
    game_id = db.Column(db.String, nullable=False)

def initialize_database():
    with app.app_context():
        db.create_all()



# Removed Template Rendering -- Made it versatile as an API, and now returns JSON
@app.route('/detections', methods=['GET', 'POST'])
def detections():
    query = request.args.get('search', '')  # For GET requests
    if request.method == 'POST':
        query = request.form.get('search', '')  # For POST requests
    else:
        detections = Detection.query.all()
        return jsonify([{"id": detection.id, "ip": detection.ip, "username": detection.username, "description": detection.description, "recorded_time": detection.recorded_time, "game_id": detection.game_id} for detection in detections])


    
if __name__ == "__main__":
    initialize_database()  # Initialize the database before the Flask app starts accepting requests
    app.run(debug=args.debug, host=args.host, port=args.port, use_reloader=False)
