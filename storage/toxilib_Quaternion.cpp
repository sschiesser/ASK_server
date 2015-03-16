
Java Class Version: 5 (49.0) JD-Web Version: 0.2.0-SNAPSHOT-20131026 JD-Core Version: 0.7.0-SNAPSHOT-20131026
package toxi.geom;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import toxi.math.InterpolateStrategy;
import toxi.math.MathUtils;

@XmlAccessorType(XmlAccessType.FIELD)
public class Quaternion
{
    public static final float DOT_THRESHOLD = 0.9995F;
    @XmlAttribute(required=true)
    public float x;
    @XmlAttribute(required=true)
    public float y;
    @XmlAttribute(required=true)
    public float z;
    @XmlAttribute(required=true)
    public float w;

    public static Quaternion createFromAxisAngle(ReadonlyVec3D paramReadonlyVec3D, float paramFloat)
    {
        paramFloat = (float)(paramFloat * 0.5D);
        float f1 = MathUtils.sin(paramFloat);
        float f2 = MathUtils.cos(paramFloat);
        Quaternion localQuaternion = new Quaternion(f2, paramReadonlyVec3D.getNormalizedTo(f1));
        return localQuaternion;
    }

    public static Quaternion createFromEuler(float paramFloat1, float paramFloat2, float paramFloat3)
    {
        paramFloat1 = (float)(paramFloat1 * 0.5D);
        paramFloat2 = (float)(paramFloat2 * 0.5D);
        paramFloat3 = (float)(paramFloat3 * 0.5D);
        float f1 = MathUtils.sin(paramFloat1);
        float f2 = MathUtils.cos(paramFloat1);
        float f3 = MathUtils.sin(paramFloat2);
        float f4 = MathUtils.cos(paramFloat2);
        float f5 = MathUtils.sin(paramFloat3);
        float f6 = MathUtils.cos(paramFloat3);
        float f7 = f2 * f4;
        float f8 = f1 * f3;
        Quaternion localQuaternion = new Quaternion();
        localQuaternion.x = (f5 * f7 - f6 * f8);
        localQuaternion.y = (f6 * f1 * f4 + f5 * f2 * f3);
        localQuaternion.z = (f6 * f2 * f3 - f5 * f1 * f4);
        localQuaternion.w = (f6 * f7 + f5 * f8);
        return localQuaternion;
        }

    public static Quaternion createFromMatrix(Matrix4x4 paramMatrix4x4)
        {
        double d1 = 0.0D;
        double[] arrayOfDouble = new double[4];
        double d2 = paramMatrix4x4.matrix[0][0] + paramMatrix4x4.matrix[1][1] + paramMatrix4x4.matrix[2][2];
        if (d2 > 0.0D)
            {
            d1 = Math.sqrt(d2 + 1.0D);
            arrayOfDouble[3] = (d1 * 0.5D);
            d1 = 0.5D / d1;
            arrayOfDouble[0] = ((paramMatrix4x4.matrix[1][2] - paramMatrix4x4.matrix[2][1]) * d1);
            arrayOfDouble[1] = ((paramMatrix4x4.matrix[2][0] - paramMatrix4x4.matrix[0][2]) * d1);
            arrayOfDouble[2] = ((paramMatrix4x4.matrix[0][1] - paramMatrix4x4.matrix[1][0]) * d1);
            }
        else
            {
            int[] arrayOfInt = { 1, 2, 0 };
            int i = 0;
            int j = 0;
            int k = 0;
            if (paramMatrix4x4.matrix[1][1] > paramMatrix4x4.matrix[0][0]) {
            i = 1;
            }
            if (paramMatrix4x4.matrix[2][2] > paramMatrix4x4.matrix[i][i]) {
            i = 2;
            }
            j = arrayOfInt[i];
            k = arrayOfInt[j];
            d1 = Math.sqrt(paramMatrix4x4.matrix[i][i] - (paramMatrix4x4.matrix[j][j] + paramMatrix4x4.matrix[k][k]) + 1.0D);
            arrayOfDouble[i] = (d1 * 0.5D);
            d1 = 0.5D / d1;
            arrayOfDouble[3] = ((paramMatrix4x4.matrix[j][k] - paramMatrix4x4.matrix[k][j]) * d1);
            arrayOfDouble[j] = ((paramMatrix4x4.matrix[i][j] + paramMatrix4x4.matrix[j][i]) * d1);
            arrayOfDouble[k] = ((paramMatrix4x4.matrix[i][k] + paramMatrix4x4.matrix[k][i]) * d1);
            }
        return new Quaternion((float)arrayOfDouble[3], (float)arrayOfDouble[0], (float)arrayOfDouble[1], (float)arrayOfDouble[2]);
        }

    public static Quaternion getAlignmentQuat(ReadonlyVec3D paramReadonlyVec3D1, ReadonlyVec3D paramReadonlyVec3D2)
        {
        Vec3D localVec3D1 = paramReadonlyVec3D1.getNormalized();
        Vec3D localVec3D2 = paramReadonlyVec3D2.cross(localVec3D1);
        float f1 = localVec3D2.magnitude() + 1.0E-4F;
        float f2 = (float)Math.atan2(f1, paramReadonlyVec3D2.dot(localVec3D1));
        return createFromAxisAngle(localVec3D2, f2);
        }

    public Quaternion()
        {
        identity();
        }

    public Quaternion(float paramFloat1, float paramFloat2, float paramFloat3, float paramFloat4)
        {
        this.w = paramFloat1;
        this.x = paramFloat2;
        this.y = paramFloat3;
        this.z = paramFloat4;
        }

    public Quaternion(float paramFloat, ReadonlyVec3D paramReadonlyVec3D)
        {
        this.x = paramReadonlyVec3D.x();
        this.y = paramReadonlyVec3D.y();
        this.z = paramReadonlyVec3D.z();
        this.w = paramFloat;
        }

    public Quaternion(Quaternion paramQuaternion)
        {
        this.w = paramQuaternion.w;
        this.x = paramQuaternion.x;
        this.y = paramQuaternion.y;
        this.z = paramQuaternion.z;
        }

    public Quaternion add(Quaternion paramQuaternion)
        {
        return new Quaternion(this.x + paramQuaternion.x, this.y + paramQuaternion.y, this.z + paramQuaternion.z, this.w + paramQuaternion.w);
        }

    public Quaternion addSelf(Quaternion paramQuaternion)
        {
        this.x += paramQuaternion.x;
        this.y += paramQuaternion.y;
        this.z += paramQuaternion.z;
        this.w += paramQuaternion.w;
        return this;
        }

    public Quaternion copy()
    {
        return new Quaternion(this.w, this.x, this.y, this.z);
    }

    public float dot(Quaternion paramQuaternion)
    {
        return this.x * paramQuaternion.x + this.y * paramQuaternion.y + this.z * paramQuaternion.z + this.w * paramQuaternion.w;
    }

    public Quaternion getConjugate()
    {
        Quaternion localQuaternion = new Quaternion();
        localQuaternion.x = (-this.x);
        localQuaternion.y = (-this.y);
        localQuaternion.z = (-this.z);
        localQuaternion.w = this.w;
        return localQuaternion;
    }

    @Deprecated
    public Matrix4x4 getMatrix()
    {
        return toMatrix4x4();
    }

    public Quaternion getNormalized()
    {
        return new Quaternion(this).normalize();
    }

    public Quaternion identity()
    {
        this.w = 1.0F;
        this.x = 0.0F;
        this.y = 0.0F;
        this.z = 0.0F;
        return this;
    }

    public Quaternion interpolateTo(Quaternion paramQuaternion, float paramFloat)
    {
        return copy().interpolateToSelf(paramQuaternion, paramFloat);
    }

    public Quaternion interpolateTo(Quaternion paramQuaternion, float paramFloat, InterpolateStrategy paramInterpolateStrategy)
    {
        return copy().interpolateToSelf(paramQuaternion, paramInterpolateStrategy.interpolate(0.0F, 1.0F, paramFloat));
    }

    public Quaternion interpolateToSelf(Quaternion paramQuaternion, float paramFloat)
    {
        float f3 = MathUtils.clip(dot(paramQuaternion), -1.0F, 1.0F);
        float f1;
        float f2;
        if (1.0D - f3 >= 1.1920928955078125E-7D)
        {
            double d1 = Math.acos(f3);
            double d2 = 1.0D / Math.sin(d1);
            f1 = (float)(Math.sin(d1 * (1.0D - paramFloat)) * d2);
            f2 = (float)(Math.sin(d1 * paramFloat) * d2);
        }
        else
        {
            f1 = 1.0F - paramFloat;
            f2 = paramFloat;
        }
        if (f3 < 0.0D)
        {
            this.w = (f1 * this.w - f2 * paramQuaternion.w);
            this.x = (f1 * this.x - f2 * paramQuaternion.x);
            this.y = (f1 * this.y - f2 * paramQuaternion.y);
            this.z = (f1 * this.z - f2 * paramQuaternion.z);
        }
        else
        {
            this.w = (f1 * this.w + f2 * paramQuaternion.w);
            this.x = (f1 * this.x + f2 * paramQuaternion.x);
            this.y = (f1 * this.y + f2 * paramQuaternion.y);
            this.z = (f1 * this.z + f2 * paramQuaternion.z);
        }
        return this;
    }

    public Quaternion interpolateToSelf(Quaternion paramQuaternion, float paramFloat, InterpolateStrategy paramInterpolateStrategy)
    {
        return interpolateToSelf(paramQuaternion, paramInterpolateStrategy.interpolate(0.0F, 1.0F, paramFloat));
    }

    public float magnitude()
    {
        return (float)Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z + this.w * this.w);
    }

    public Quaternion multiply(Quaternion paramQuaternion)
    {
        Quaternion localQuaternion = new Quaternion();
        localQuaternion.w = (this.w * paramQuaternion.w - this.x * paramQuaternion.x - this.y * paramQuaternion.y - this.z * paramQuaternion.z);
        localQuaternion.x = (this.w * paramQuaternion.x + this.x * paramQuaternion.w + this.y * paramQuaternion.z - this.z * paramQuaternion.y);
        localQuaternion.y = (this.w * paramQuaternion.y + this.y * paramQuaternion.w + this.z * paramQuaternion.x - this.x * paramQuaternion.z);
        localQuaternion.z = (this.w * paramQuaternion.z + this.z * paramQuaternion.w + this.x * paramQuaternion.y - this.y * paramQuaternion.x);
        return localQuaternion;
    }

    public Quaternion normalize()
    {
        float f = (float)Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z + this.w * this.w);
        if (f > 1.1920929E-7F)
        {
            f = 1.0F / f;
            this.x *= f;
            this.y *= f;
            this.z *= f;
            this.w *= f;
        }
        return this;
    }

    public Quaternion scale(float paramFloat)
    {
        return new Quaternion(this.x * paramFloat, this.y * paramFloat, this.z * paramFloat, this.w * paramFloat);
    }

    public Quaternion scaleSelf(float paramFloat)
    {
        this.x *= paramFloat;
        this.y *= paramFloat;
        this.z *= paramFloat;
        this.w *= paramFloat;
        return this;
    }

    public Quaternion set(float paramFloat1, float paramFloat2, float paramFloat3, float paramFloat4)
    {
        this.w = paramFloat1;
        this.x = paramFloat2;
        this.y = paramFloat3;
        this.z = paramFloat4;
        return this;
    }

    public Quaternion set(float paramFloat, Vec3D paramVec3D)
    {
        this.w = paramFloat;
        this.x = paramVec3D.x;
        this.y = paramVec3D.y;
        this.z = paramVec3D.z;
        return this;
    }

    public Quaternion set(Quaternion paramQuaternion)
    {
        this.w = paramQuaternion.w;
        this.x = paramQuaternion.x;
        this.y = paramQuaternion.y;
        this.z = paramQuaternion.z;
        return this;
    }

    public Quaternion sub(Quaternion paramQuaternion)
    {
        return new Quaternion(this.x - paramQuaternion.x, this.y - paramQuaternion.y, this.z - paramQuaternion.z, this.w - paramQuaternion.w);
    }

    public Quaternion subSelf(Quaternion paramQuaternion)
    {
        this.x -= paramQuaternion.x;
        this.y -= paramQuaternion.y;
        this.z -= paramQuaternion.z;
        this.w -= paramQuaternion.w;
        return this;
    }

    public float[] toArray()
    {
        return new float[] { this.w, this.x, this.y, this.z };
    }

    public float[] toAxisAngle()
    {
        float[] arrayOfFloat = new float[4];
        float f = (float)Math.sqrt(1.0F - this.w * this.w);
        if (f < 1.1920929E-7F) {
            f = 1.0F;
        } else {
            f = 1.0F / f;
        }
        arrayOfFloat[0] = ((float)Math.acos(this.w) * 2.0F);
        arrayOfFloat[1] = (this.x * f);
        arrayOfFloat[2] = (this.y * f);
        arrayOfFloat[3] = (this.z * f);
        return arrayOfFloat;
    }

    public Matrix4x4 toMatrix4x4()
    {
    return toMatrix4x4(new Matrix4x4());
    }

    public Matrix4x4 toMatrix4x4(Matrix4x4 paramMatrix4x4)
    {
    float f1 = this.x + this.x;
    float f2 = this.y + this.y;
    float f3 = this.z + this.z;
    float f4 = this.x * f1;
    float f5 = this.x * f2;
    float f6 = this.x * f3;
    float f7 = this.y * f2;
    float f8 = this.y * f3;
    float f9 = this.z * f3;
    float f10 = this.w * f1;
    float f11 = this.w * f2;
    float f12 = this.w * f3;
    return paramMatrix4x4.set(1.0F - (f7 + f9), f5 - f12, f6 + f11, 0.0D, f5 + f12, 1.0F - (f4 + f9), f8 - f10, 0.0D, f6 - f11, f8 + f10, 1.0F - (f4 + f7), 0.0D, 0.0D, 0.0D, 0.0D, 1.0D);
    }

    public String toString()
    {
    StringBuffer localStringBuffer = new StringBuffer(48);
    localStringBuffer.append("{axis: [").append(this.x).append(",").append(this.y).append(",").append(this.z).append("], w: ").append(this.w).append("}");
    return localStringBuffer.toString();
    }
}