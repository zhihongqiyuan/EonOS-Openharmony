/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * Class Test
 * 
 * @since 2024/5/8
 */
public class ClassTest {
    public static void main(String[] args) {
        Rectangle rect = new Rectangle(5.0, 3.0);

        double area = rect.getArea();
        System.out.println("The area of the rectangle is: " + area);
    }
}

class Rectangle {
    private double length;
    private double width;

    /**
     * Rectangle
     * 
     * @param length length of Rectangle
     * @param width width of Rectangle
     */
    public Rectangle(double length, double width) {
        this.length = length;
        this.width = width;
    }

    /**
     * getArea
     * 
     * @return double area
     */
    public double getArea() {
        return length * width;
    }
}