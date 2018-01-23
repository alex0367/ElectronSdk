class AgoraRender2{
    constructor(){
        this.gl = undefined;
        this.program = undefined;
        this.positionLocation = undefined;
        this.texCoordLocation = undefined;
        this.yTexture = undefined;
        this.uTexture = undefined;
        this.vTexture = undefined;
        this.texCoordBuffer = undefined;
        this.surfaceBuffer = undefined;
        this.view = undefined;
        this.mirrorView = false;
        this.container = undefined;
        this.canvas = undefined;
        this.renderImageCount = 0;
        this.initWidth = 0;
        this.initHeight = 0;
        this.initRotation = 0;
        this.canvasUpdated = false;
        this.clientWidth = 0;
        this.clientHeight = 0;

        this.top = 0;
        this.bottom = 0;
        this.left = 0;
        this.right = 0;
        this.xWidth = 0;
        this.xHeight = 0;
        this.float32Array = undefined;
    }

    start(view, onFailure) {
        this.initCanvas(view, this.mirrorView, view.clientWidth, view.clientHeight, this.initRotation, onFailure);
    }

    stop(){
        this.gl = undefined;
        this.program = undefined;
        this.positionLocation = undefined;
        this.texCoordLocation = undefined;

        this.deleteTexture(this.yTexture);
        this.deleteTexture(this.uTexture);
        this.deleteTexture(this.vTexture);
        this.yTexture = undefined;
        this.uTexture = undefined;
        this.vTexture = undefined;

        this.deleteBuffer(this.texCoordBuffer);
        this.deleteBuffer(this.surfaceBuffer);
        this.texCoordBuffer = undefined;
        this.surfaceBuffer = undefined;

        if(this.container){
            this.container.removeChild(this.canvas);
        }

        if(this.view){
            this.view.removeChild(this.container);
        }

        this.canvas = undefined;
        this.container = undefined;
        this.view = undefined;
        this.mirrorView = false;
    }

    renderImage(image){
        if(!this.gl){
            console.log("invalid gl.");
            return;
        }

        if(image.width != this.initWidth ||
           image.height != this.initHeight ||
           image.rotation != this.initRotation ||
           image.mirror != this.mirrorView) {
               let view = this.view;
               this.stop();
               this.initCanvas(view, image.mirror, image.width, image.height, image.rotation, function(e){
                   console.error("init canvas " + image.width + "*" + image.height + " rotation " + image.rotation + " failed. " + e);
               });
           }

        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.texCoordBuffer);

        let xWidth = image.width + image.left + image.right;
        let xHeight = image.height + image.top + image.bottom;
        if((image.left != this.left) ||
           (image.right != this.right) ||
           (image.top != this.top) ||
           (image.bottom != this.bottom) ||
           (xWidth != this.xWidth) ||
           (xHeight != this.xHeight)){
               console.log("update floatarray");
               this.left = image.left;
               this.right = image.right;
               this.top = image.top;
               this.bottom = image.bottom;              
               this.xWidth = xWidth;
               this.xHeight = xHeight; 
               this.float32Array = new Float32Array([
                image.left/xWidth, image.bottom/xHeight,
                1 - image.right/xWidth, image.bottom/xHeight,
                image.left/xWidth, 1 - image.top/xHeight,
                image.left/xWidth, 1 - image.top/xHeight,
                1 - image.right/xWidth, image.bottom/xHeight,
                1 - image.right/xWidth, 1 - image.top/xHeight
            ]);
           }

        this.gl.bufferData(this.gl.ARRAY_BUFFER, this.float32Array, this.gl.STATIC_DRAW);
        this.gl.enableVertexAttribArray(this.texCoordLocation);
        this.gl.vertexAttribPointer(this.texCoordLocation, 2, this.gl.FLOAT, false, 0, 0);

        this.uploadYuv(xWidth, xHeight, image.yplane, image.uplane, image.vplane);
        this.updateCanvas(image.rotation, image.width, image.height);

        this.gl.drawArrays(this.gl.TRIANGLES, 0, 6);
        this.renderImageCount ++;
    }

    uploadYuv(width, height, yplane, uplane, vplane){
        this.gl.activeTexture(this.gl.TEXTURE0);
        this.gl.bindTexture(this.gl.TEXTURE_2D, this.yTexture);
        this.gl.texImage2D(this.gl.TEXTURE_2D, 0, this.gl.LUMINANCE, width, height, 0, this.gl.LUMINANCE, this.gl.UNSIGNED_BYTE, yplane);

        let e = this.gl.getError();
        if(e != this.gl.NO_ERROR){
            console.log("upload y plane ", width, height, yplane.byteLength, " error", e);
        }

        this.gl.activeTexture(this.gl.TEXTURE1);
        this.gl.bindTexture(this.gl.TEXTURE_2D, this.uTexture);
        this.gl.texImage2D(this.gl.TEXTURE_2D, 0, this.gl.LUMINANCE, width/2, height/2, 0, this.gl.LUMINANCE, this.gl.UNSIGNED_BYTE, uplane);
        e = this.gl.getError();
        if(e != this.gl.NO_ERROR){
            console.log("upload u plane ", width, height, uplane.byteLength, " error", e);
        }

        this.gl.activeTexture(this.gl.TEXTURE2);
        this.gl.bindTexture(this.gl.TEXTURE_2D, this.vTexture);
        this.gl.texImage2D(this.gl.TEXTURE_2D, 0, this.gl.LUMINANCE, width/2, height/2, 0, this.gl.LUMINANCE, this.gl.UNSIGNED_BYTE, vplane);
        e = this.gl.getError();
        if(e != this.gl.NO_ERROR) {
            console.log("upload v plane J", width, height, vplane.byteLength, " error", e);
        }
    }

    deleteBuffer(buffer){
        if(buffer && this.gl) {
            this.gl.deleteBuffer(buffer);
        }
    }

    deleteTexture(texture){
        if(texture && this.gl){
            this.gl.deleteTexture(texture);
        }
    }

    static vertexShaderSource(){
        return "attribute vec2 a_position;" +
        "attribute vec2 a_texCoord;" +
        "uniform vec2 u_resolution;" +
        "varying vec2 v_texCoord;" +
        "void main() {" +
        "vec2 zeroToOne = a_position / u_resolution;" +
        "   vec2 zeroToTwo = zeroToOne * 2.0;" +
        "   vec2 clipSpace = zeroToTwo - 1.0;" +
        "   gl_Position = vec4(clipSpace * vec2(1, -1), 0, 1);" +
        "v_texCoord = a_texCoord;" +
        "}";
    } 
    static yuvShaderSource(){
        return "precision mediump float;" +
        "uniform sampler2D Ytex;" +
        "uniform sampler2D Utex,Vtex;" +
        "varying vec2 v_texCoord;" +
        "void main(void) {" +
        "  float nx,ny,r,g,b,y,u,v;" +
        "  mediump vec4 txl,ux,vx;" +
        "  nx=v_texCoord[0];" +
        "  ny=v_texCoord[1];" +
        "  y=texture2D(Ytex,vec2(nx,ny)).r;" +
        "  u=texture2D(Utex,vec2(nx,ny)).r;" +
        "  v=texture2D(Vtex,vec2(nx,ny)).r;" +
        "  y=1.1643*(y-0.0625);" +
        "  u=u-0.5;" +
        "  v=v-0.5;" +
        "  r=y+1.5958*v;" +
        "  g=y-0.39173*u-0.81290*v;" +
        "  b=y+2.017*u;" +
        "  gl_FragColor=vec4(r,g,b,1.0);" +
        "}";
    }

    initCanvas(view, mirror, width, height, rotation, onFailure){
        this.clientWidth = view.clientWidth;
        this.clientHeight = view.clientHeight;

        this.view = view;
        this.mirrorView = mirror;
        this.canvasUpdated = false;

        this.container = document.createElement("div");
        this.container.style.width = "100%";
        this.container.style.height = "100%";
        this.container.style.display = "flex";
        this.container.style.justifyContent = "center";
        this.container.style.alignItems = "center";
        this.view.appendChild(this.container);

        this.canvas = document.createElement("canvas");
        if(rotation == 0 || rotation == 180) {
            this.canvas.width = width;
            this.canvas.height = height;
        }else{
            this.canvas.width = height;
            this.canvas.height = width;
        }

        this.initWidth = width;
        this.initHeight = height;
        this.initRotation = rotation;
        if(this.mirrorView) {
            this.canvas.style.transform = "rotateY(180deg)";
        }
        this.container.appendChild(this.canvas);
        try{
            this.gl = this.canvas.getContext("webgl") || this.canvas.getContext("experimental-webgl");
        }catch(e){
            console.log(e);
        }

        if(!this.gl){
            this.gl = undefined;
            onFailure({error: "Browser not support! no WebGL detected."});
            return;
        }

        this.gl.clearColor(0.0, 0.0, 0.0, 1.0);
        this.gl.enable(this.gl.DEPTH_TEST);
        this.gl.depthFunc(this.gl.LEQUAL);
        this.gl.clear(this.gl.COLOR_BUFFER_BIT | this.gl.DEPTH_BUFFER_BIT);

        this.program = createProgramFromSources(this.gl, [AgoraRender2.vertexShaderSource(), AgoraRender2.yuvShaderSource()]);
        this.gl.useProgram(this.program);
        this.initTextures();
    }

    initTextures(){
        this.positionLocation = this.gl.getAttribLocation(this.program, "a_position");
        this.texCoordLocation = this.gl.getAttribLocation(this.program, "a_texCoord");

        this.surfaceBuffer = this.gl.createBuffer();
        this.texCoordBuffer = this.gl.createBuffer();

        this.gl.activeTexture(this.gl.TEXTURE0);
        this.yTexture = this.gl.createTexture();
        this.gl.bindTexture(this.gl.TEXTURE_2D, this.yTexture);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_WRAP_S, this.gl.CLAMP_TO_EDGE);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_WRAP_T, this.gl.CLAMP_TO_EDGE);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_MIN_FILTER, this.gl.NEAREST);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_MAG_FILTER, this.gl.NEAREST);

        this.gl.activeTexture(this.gl.TEXTURE1);
        this.uTexture = this.gl.createTexture();
        this.gl.bindTexture(this.gl.TEXTURE_2D, this.uTexture);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_WRAP_S, this.gl.CLAMP_TO_EDGE);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_WRAP_T, this.gl.CLAMP_TO_EDGE);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_MIN_FILTER, this.gl.NEAREST);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_MAG_FILTER, this.gl.NEAREST);

        this.gl.activeTexture(this.gl.TEXTURE2);
        this.vTexture = this.gl.createTexture();
        this.gl.bindTexture(this.gl.TEXTURE_2D, this.vTexture);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_WRAP_S, this.gl.CLAMP_TO_EDGE);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_WRAP_T, this.gl.CLAMP_TO_EDGE);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_MIN_FILTER, this.gl.NEAREST);
        this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_MAG_FILTER, this.gl.NEAREST);

        let y = this.gl.getUniformLocation(this.program, "Ytex");
        this.gl.uniform1i(y, 0);

        let u = this.gl.getUniformLocation(this.program, "Utex");
        this.gl.uniform1i(u, 1);

        let v = this.gl.getUniformLocation(this.program, "Vtex");
        this.gl.uniform1i(v, 2);
    }

    updateCanvas(rotation, width, height){
        if(this.canvasUpdated){
            return;
        }

        this.canvas.style.width = "100%";
        this.canvas.style.height = "100%";

        try{
            if(rotation === 0 || rotation === 180){
                if(this.clientWidth/this.clientHeight > width/height){
                    this.canvas.style.width = this.clientHeight * width/height + 'px';
                }else if(this.clientWidth / this.clientHeight < width/height){
                    this.canvas.style.height = this.clientWidth * height/width + 'px';
                }
            }else{
                if(this.clientHeight/this.clientWidth > width/height){
                    this.canvas.style.height = this.clientWidth * width/height + 'px';
                }else if(this.clientHeight/this.clientWidth < width/height){
                    this.canvas.style.width = this.clientHeight * height/width + 'px';
                }
            }
        }catch(e){
            console.log("updateCanvas 00001 gone" + this.canvas);
            console.log(this);
            console.error(e);
            return;
        }

        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.surfaceBuffer);
        this.gl.enableVertexAttribArray(this.positionLocation);
        this.gl.vertexAttribPointer(this.positionLocation, 2, this.gl.FLOAT, false, 0, 0);

        let p1 = {x:0, y:0};
        let p2 = {x: width, y: 0};
        let p3 = {x: width, y: height};
        let p4 = {x: 0, y: height};
        let pp1 = p1, pp2 = p2, pp3 = p3, pp4 = p4;
        switch(rotation){
        case 0:
            break;
        case 90:
            pp1 = p2;
            pp2 = p3;
            pp3 = p4;
            pp4 = p1;
            break;
        case 180:
            pp1 = p3;
            pp2 = p4;
            pp3 = p1;
            pp4 = p2;
            break;
        case 270:
            pp1 = p4;
            pp2 = p1;
            pp3 = p2;
            pp4 = p3;
            break;
        default:
        }
        this.gl.bufferData(this.gl.ARRAY_BUFFER, new Float32Array([
            pp1.x, pp1.y,
            pp2.x, pp2.y,
            pp4.x, pp4.y,
            pp4.x, pp4.y,
            pp2.x, pp2.y,
            pp3.x, pp3.y
        ]), this.gl.STATIC_DRAW);

        let resolutionLocation = this.gl.getUniformLocation(this.program, "u_resolution");
        this.gl.uniform2f(resolutionLocation, width, height);
    }
}