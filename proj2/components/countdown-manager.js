// components/countdown-manager.js
/**
 * @component countdown-manager
 * @description Manages turn-based countdown system between two cannons
 */
AFRAME.registerComponent('countdown-manager', {
    init: function() {
      this.isGreenTurn = true; // Start with green
      this.countdown = 5;
      this.isRunning = false;
      
      // Get references to both text elements
      this.greenText = document.querySelector('#greenTimer');
      this.blueText = document.querySelector('#blueTimer');

      // Get cannonball element
      this.cannonball = document.querySelector('#cannonball');

      // Get cannons
      this.greenCannon = document.querySelector('#greenCannon');
      this.blueCannon = document.querySelector('#blueCannon');

      // Create two position vectors
      this.greenCannonPos = new THREE.Vector3();
      this.blueCannonPos = new THREE.Vector3();

      // Initialize animation state
      this.animationActive = false;
      
      // Start when either marker is found
      this.el.sceneEl.addEventListener('markerFound', () => {
        if (!this.isRunning) {
          this.startCountdown();
        }
      });
    },
  
    startCountdown: function() {
      this.isRunning = true;
      this.countdown = 5;
      
      const timer = setInterval(() => {
        // Update current player's text
        if (this.isGreenTurn) {
          this.greenText.setAttribute('value', this.countdown.toString());
          this.blueText.setAttribute('value', '');
        } else {
          this.blueText.setAttribute('value', this.countdown.toString());
          this.greenText.setAttribute('value', '');
        }
        
        this.countdown--;
        
        if (this.countdown < 0) {
          // Shoot cannonball
          if (!this.animationActive) {
            this.animationActive = true;
            this.shootCannonBall();
          }
          else {
            // TODO:: Might be able to remove this code
            console.log("animation cycle not active ??")
            this.cannonball.setAttribute('visible', false);
            this.cannonball.removeAttribute('animation');
            this.animationActive = false;
          }
          
          // Switch turns
          this.isGreenTurn = !this.isGreenTurn;
          this.countdown = 5;
          
          clearInterval(timer);
          setTimeout(() => {
            this.startCountdown();
          }, 500);
        }
      }, 1000);
    },

    shootCannonBall: function() {
      console.log("Shoot cannon")
      // Get world positions of both markers
      this.greenCannon.object3D.getWorldPosition(this.greenCannonPos);
      this.blueCannon.object3D.getWorldPosition(this.blueCannonPos);

      // Make cannonball visible
      this.cannonball.setAttribute('visible', true);
      // Shoot right if green, left if blue
      const startPos = this.isGreenTurn ? this.greenCannonPos : this.blueCannonPos;
      const direction =  this.isGreenTurn ? 10 : -10;
      
      // Set up animation for the spear
      this.cannonball.setAttribute('animation', {
        property: 'position',
        from: `${startPos.x} ${startPos.y} ${startPos.z}`,
        to: `${startPos.x + direction} ${startPos.y} ${startPos.z}`,
        dur: 4000,
        dir: 'alternate',
        loop: true,
        easing: 'easeInOutQuad'
      });
      
      // TODO:: Add logic to stop make ball inivisble once it hits the other cannon

      setTimeout(() => {
        this.animationActive = false;
        this.cannonball.setAttribute('visible', false);
        this.cannonball.removeAttribute('animation');
      }, 4000);
    }
  });